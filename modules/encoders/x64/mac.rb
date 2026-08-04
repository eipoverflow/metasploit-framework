
##
# This module requires Metasploit: https://metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

class MetasploitModule < Msf::Encoder::Xor
  
  def initialize
    super(
      'Name' => 'MAC Encoder',
      'Description' => '',
      'Author' => [ 'msutovsky-r7' ],
      'Arch' => ARCH_X64,
      'License' => MSF_LICENSE
      )
  end

  def self.mac_obfluscate(payload)
    bytes = payload.bytes
    # Pad to a multiple of 4 bytes
    remainder = bytes.length % 6
    bytes += [255] * (6 - remainder) if remainder != 0

    # add buffer string because metasm likes to mess up end of array of strings for some reason
    bytes += [255] * 6

    chunks = []
    i = 0
    bytes.each_slice(6) do |quad|
      chunks << quad.map { |b| format('%02X', b) }.join('-')
      i += 1
    end
    chunks
  end

  def encode_block(state, block)
  end
  
  def decoder_stub(state)
  end
end
