require 'erb'
require 'metasploit/framework/compiler/windows'
require 'metasploit/framework/compiler/pe'

module Msf::Obfuscation::ExeTemplate
  
  def self.exe_template_x64_pe(framework, code, opts); 

    if opts[:dynamic_obfuscation]
      source_c = pe_x64_template_obfus(code)
    else  
      source_c = pe_x64_template(code)
    end
    
    tmp =  Tempfile.new(["payload_win_x64"])
    
    compile_opts = {
        f_name: tmp.path,
        strip_symbols: true,
        compile_options: ' -masm=intel -w -mwindows '
      }
    
    comp_obj = Metasploit::Framework::Compiler::Mingw::X64.new(compile_opts)
    comp_obj.compile_c(source_c)
    pe = File.binread("#{tmp.path}.exe")

    pe_parser = Metasploit::Framework::Compiler::Pe.new
    pe_parser.load_pe(pe)

    pe_parser.build
  end

  def self.exe_template_x64_dll(framework, code, opts) 
    source_c = dll_x64_template(code) 

    Metasploit::Framework::Compiler::Windows.compile_c(source_c, :dll, Metasm::X86_64.new)
  end

  private


  def self.dll_x64_template(code)
    template_path = File.join(Msf::Config.data_directory, 'templates','src','pe','dll', 'template_x64_windows.erb')

    payload_length = code.length
    payload = code.bytes.map { |b| '\\x%02x' % b }.join

    template = ERB.new(File.read(template_path))
    template.result(binding)
  end

  def self.dll_x64_template_obfus(code)
  end

  def self.pe_x64_template(code)
    template_path = File.join(Msf::Config.data_directory, 'templates','src','pe','exe', 'template_x64_windows.erb')

    payload_length = code.length
    random_sleep = (24000..48000).to_a.sample
    payload = code.bytes.map { |b| '\\x%02x' % b }.join

    template = ERB.new(File.read(template_path))
    template.result(binding)
  end

  def self.pe_x64_template_obfus(code)
    template_path = File.join(Msf::Config.data_directory, 'templates', 'src', 'pe','exe', 'template_x64_windows_xor_mac.erb')

    v = build_xor_mac_vars(code)
    encryption_rounds = v[:encryption_rounds]
    encrypted_payload_length = v[:encrypted_payload_length]
    encrypted_payload = v[:encrypted_payload]
    encoded_payload_length = v[:encoded_payload_length]
    control_bytes = v[:control_bytes]

    template = ERB.new(File.read(template_path))
    template.result(binding)
  end

  def self.build_xor_mac_vars(code)
    #TODO: remove xor encryption as encoding seems to be sufficient and it actually breaks the decoding routine when including obfuscator.h

    encryption_rounds = 0
    xor_keys = encryption_rounds.times.map { rand(256) }

    control_bytes = [rand(256)]
    encryption_rounds.times do |i|
      control_bytes.append(control_bytes.last ^ xor_keys[i])
      code = code.bytes.map { |b| b ^ xor_keys[i] }.pack('C*')
    end

    code.prepend(control_bytes.last.chr)
    control_bytes = control_bytes.reverse.drop(1)

    encrypted_payload_length = code.bytesize
    encrypted_payload = mac_obfluscate(code)
    encoded_payload_length = encrypted_payload.length
    encrypted_payload = encrypted_payload.map { |s| s + "\0" }.join.bytes.map { |b| '\\x%02X' % b }.join
    control_bytes = control_bytes.map { |b| '\\x%02x' % b }.join

    {
      code: code,
      encryption_rounds: encryption_rounds,
      encrypted_payload_length: encrypted_payload_length,
      encrypted_payload: encrypted_payload,
      encoded_payload_length: encoded_payload_length,
      control_bytes: control_bytes
    }
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

end
