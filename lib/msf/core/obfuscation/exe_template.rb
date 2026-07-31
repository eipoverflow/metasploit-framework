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
    
    tmp = Tempfile.new(["payload_win_x64"])
    
    compile_opts = {
        f_name: tmp.path,
        cpp_exe:true,
        strip_symbols: true,
        compile_options: ' -masm=intel -w -mwindows -lstdc++ -static '
      }
    
    comp_obj = Metasploit::Framework::Compiler::Mingw::X64.new(compile_opts)
    comp_obj.compile_c(source_c)
    File.binread("#{tmp.path}.exe")

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
    section_name = "rsc"
    random_sleep = (24000..48000).to_a.sample
    payload = code.bytes.map { |b| '\\x%02x' % b }.join

    template = ERB.new(File.read(template_path))
    template.result(binding)
  end

  def self.pe_x64_template_obfus(code)
    template_path = File.join(Msf::Config.data_directory, 'templates', 'src', 'pe','exe', 'template_x64_windows_xor_mac.erb')

    v = build_mac_vars(code)
    encoded_payload_length = v[:encoded_payload_length]

    payload_length = code.length; 
    encoded_payload = v[:encoded_payload]

    template = ERB.new(File.read(template_path))
    template.result(binding)
  end

  def self.build_mac_vars(code)
    encoded_payload = mac_obfluscate(code)
    encoded_payload_length = encoded_payload.length
    encoded_payload = encoded_payload.map { |s| s + "\0" }.join.bytes.map { |b| '\\x%02X' % b }.join

    {
      encoded_payload_length: encoded_payload_length,
      encoded_payload: encoded_payload,
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
