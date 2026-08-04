require 'erb'
require 'metasploit/framework/compiler/windows'
require 'metasploit/framework/compiler/pe'

module Msf::Obfuscation::ExeTemplate
  
  def self.exe_template_x64_pe(framework, code, opts)
    compile_template_x64(code, opts,  {
        is_cpp: true,
        strip_symbols: true,
        compile_options: ' -masm=intel -w -mwindows -lstdc++ -static '
      }, 'exe')
  end

  #TODO: for some weird reason, the DLL compiled has pretty visible functions such as PlayDeadForDefender.
  # This needs to be fixed.
  def self.exe_template_x64_dll(framework, code, opts) 
    compile_template_x64(code, opts,  {
         is_cpp: true,
         is_dll: true,
         strip_symbols: true,
         compile_options: ' -masm=intel -w -mwindows -lstdc++ -static -shared '
       }, 'dll')
  end
  
  def self.exe_template_x86_pe(framework, code, opts)
    compile_template_x86(code, opts,  {
        is_cpp: true,
        strip_symbols: true,
        compile_options: ' -masm=intel -w -mwindows -lstdc++ -static '
      }, 'exe')
  end

  #TODO: x86 DLL needs to be tested
  def self.exe_template_x86_dll(framework, code, opts) 
    compile_template_x86(code, opts,  {
         is_cpp: true,
         is_dll: true,
         strip_symbols: true,
         compile_options: ' -masm=intel -w -mwindows -lstdc++ -static -shared '
       }, 'dll')
  end

  private
  
  # TODO: encryption needs to be tested, encryption keys that are set by user needs to be set as well.
  def self.apply_encryption(code, opts)
    case opts[:encryption_format]
      when 'rc4'
        opts[:encryption_key] ||= Rex::Text.rand_text_alpha(32) 
      when 'xor'
        opts[:encryption_key] ||= Rex::Text.rand_text_alpha(1) 
      else
    end
    
    ::Msf::Simple::Buffer.encrypt_buffer(code, { :format => opts[:encryption_format], :key => opts[:encryption_key] })   
  end
  
  def self.compile_template_x64(code, opts, compile_opts, exe_format)

    opts[:plaintext_length] = code.length
    
    code = apply_encryption(code, opts) unless opts[:encryption_format].nil?
    
    case exe_format
      when 'exe'
        source_c = pe_x64_template(code, opts)
      when 'dll'
        source_c = dll_x64_template(code, opts)
    end
    
    tmp = Tempfile.new(["payload_win_x64"])

    compile_opts[:f_name] = tmp.path
    
    comp_obj = Metasploit::Framework::Compiler::Mingw::X64.new(compile_opts)

    compilation_res = comp_obj.compile_c(source_c)
      
    unless compilation_res.blank?
      comp_obj.cleanup_files 
      raise ::Exception, compilation_res 
    end
    
    case exe_format
      when 'exe'
        payload_content = File.binread("#{tmp.path}.exe")
      when 'dll'
        payload_content = File.binread("#{tmp.path}.dll")
    end

    comp_obj.cleanup_files 
    payload_content
  end
  
  def self.compile_template_x86(code, opts, compile_opts, exe_format)

    opts[:plaintext_length] = code.length
    
    code = apply_encryption(code, opts) unless opts[:encryption_format].nil?
    
    case exe_format
      when 'exe'
        source_c = pe_x86_template(code, opts)
      when 'dll'
        source_c = dll_x86_template(code, opts)
    end
    
    tmp = Tempfile.new(["payload_win_x86"])

    compile_opts[:f_name] = tmp.path
    
    comp_obj = Metasploit::Framework::Compiler::Mingw::X86.new(compile_opts)

    compilation_res = comp_obj.compile_c(source_c)
      
    unless compilation_res.blank?
      comp_obj.cleanup_files 
      raise ::Exception, compilation_res 
    end
    
    case exe_format
      when 'exe'
        payload_content = File.binread("#{tmp.path}.exe")
      when 'dll'
        payload_content = File.binread("#{tmp.path}.dll")
    end

    comp_obj.cleanup_files 
    payload_content
  end


  def self.load_template(code,template_path, opts)

    payload_length = code.length
    section_name = "rsc" # TODO: possibly randomize this?
    random_sleep = (24000..48000).to_a.sample
    payload = code.bytes.map { |b| '\\x%02x' % b }.join

    case opts[:encryption_format]
      when 'rc4'
        payload_declaration = %<OBF_STRING_RC4("#{payload}", #{payload_length}, #{opts[:plaintext_length]}, "#{opts[:encryption_key]}")>
      when 'xor'
        payload_declaration = %<OBF_STRING_XOR("#{payload}", #{payload_length}, "#{opts[:key]}")>
      else
        payload_declaration = %<OBF_STRING("#{payload}", #{payload_length})>
    end

    template = ERB.new(File.read(template_path))
    template.result(binding)
  end

  def self.dll_x64_template(code, opts)
    load_template(code, File.join(Msf::Config.data_directory, 'templates','src','pe','dll', 'template_x64_windows.erb'), opts)
  end

  def self.pe_x64_template(code, opts)
    load_template(code, File.join(Msf::Config.data_directory, 'templates','src','pe','exe', 'template_x64_windows.erb'), opts)
  end
  
  def self.dll_x86_template(code, opts)
    load_template(code, File.join(Msf::Config.data_directory, 'templates','src','pe','dll', 'template_x86_windows.erb'), opts)
  end

  def self.pe_x86_template(code, opts)
    load_template(code, File.join(Msf::Config.data_directory, 'templates','src','pe','exe', 'template_x86_windows.erb'), opts)
  end

end
