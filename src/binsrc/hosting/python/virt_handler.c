static char *virt_handler =
"#\n"
"#  virt_handler.py\n"
"#\n"
"#  $Id$\n"
"#\n"
"#  python proxy for OpenLink python plugin\n"
"#  \n"
"#  This file is part of the OpenLink Software Virtuoso Open-Source (VOS)\n"
"#  project.\n"
"#  \n"
"#  Copyright (C) 1998-2012 OpenLink Software\n"
"#  \n"
"#  This project is free software; you can redistribute it and/or modify it\n"
"#  under the terms of the GNU General Public License as published by the\n"
"#  Free Software Foundation; only version 2 of the License, dated June 1991.\n"
"#  \n"
"#  This program is distributed in the hope that it will be useful, but\n"
"#  WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU\n"
"#  General Public License for more details.\n"
"#  \n"
"#  You should have received a copy of the GNU General Public License along\n"
"#  with this program; if not, write to the Free Software Foundation, Inc.,\n"
"#  51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA\n"
"#  \n"
"#  \n"
"\n"
"import os;\n"
"import sys;\n"
"import traceback;\n"
"\n"
"class VirtNullIO:\n"
"    def tell(self): return 0\n"
"    def read(self, n = -1): return \"\"\n"
"    def readline(self, length = None): return \"\"\n"
"    def readlines(self): return []\n"
"    def write(self, s): pass\n"
"    def writelines(self, list):\n"
"        self.write(\"\".join(list))\n"
"    def isatty(self): return 0\n"
"    def flush(self): pass\n"
"    def close(self): pass\n"
"    def seek(self, pos, mode = 0): pass\n"
"\n"
"\n"
"class VirtCGIStdin(VirtNullIO):\n"
"    def __init__(self, init_val):\n"
"        self.pos = 0\n"
"        # note that self.buf sometimes contains leftovers\n"
"        # that were read, but not used when readline was used\n"
"        self.buf = init_val\n"
"\n"
"    def read(self, n = -1):\n"
"        if n <= 0:\n"
"            return \"\"\n"
"\n"
"        n2 = n + self.pos;\n"
"        if self.buf:\n"
"            s = self.buf[self.pos:n2]\n"
"            n = n - len(s)\n"
"        else:\n"
"            s = \"\"\n"
"        self.pos = self.pos + len(s)\n"
"        return s\n"
"\n"
"    def readlines(self):\n"
"        s = (self.buf).split('\\n')\n"
"        return map(lambda s: s + '\\n', s)\n"
"\n"
"    def readline(self, n = -1):\n"
"\n"
"        if n == 0:\n"
"            return \"\"\n"
"\n"
"        # look for \\n in the buffer\n"
"        i = self.buf.find('\\n')\n"
"        if i == -1:\n"
"            i = len (self.buf) - 1\n"
"        # carve out the piece, then shorten the buffer\n"
"        result = self.buf[:i+1]\n"
"        self.buf = self.buf[i+1:]\n"
"        self.pos = self.pos + len(result)\n"
"        return result\n"
"        \n"
"\n"
"class VirtCGIStdout(VirtNullIO):\n"
"\n"
"    def __init__(self):\n"
"        self.pos = 0\n"
"        self.headers_sent = 0\n"
"        self.headers = \"\"\n"
"        self.req_text = \"\"\n"
"	self.html_mode=0\n"
"        \n"
"    def write(self, s):\n"
"\n"
"        if not s: return\n"
"\n"
"        if self.html_mode and not self.headers_sent:\n"
"            self.headers = self.headers + s\n"
"\n"
"            headers_over = 0\n"
"\n"
"            ss = self.headers.split('\\r\\n\\r\\n', 1)\n"
"            if len(ss) < 2:\n"
"                ss = self.headers.split('\\n\\n', 1)\n"
"                if len(ss) >= 2:\n"
"                    headers_over = 1\n"
"            else:\n"
"                headers_over = 1\n"
"                    \n"
"            if headers_over:\n"
"                self.headers_sent = 1\n"
"\n"
"        else:\n"
"            self.req_text = self.req_text + s\n"
"        \n"
"        self.pos = self.pos + len(s)\n"
"\n"
"    def tell(self): return self.pos\n"
"\n"
"    def get_headers(self) : return self.headers;\n"
"    def get_body(self) : return self.req_text;\n"
"    def set_html_mode(self) : self.html_mode=1;\n"
"\n"
"\n"
"def setup_cgi(env, stdin_txt, new_stdout, new_stderr):\n"
"\n"
"    save_env = os.environ.copy()\n"
"    if env.has_key (\"__VIRT_CGI\") and env[\"__VIRT_CGI\"] == '1':\n"
"        new_stdout.set_html_mode ();\n"
"    \n"
"    si = sys.stdin\n"
"    so = sys.stdout\n"
"    sr = sys.stderr\n"
"\n"
"    os.environ.update(env)\n"
" \n"
"    sys.stdout = new_stdout\n"
"    sys.stdin = VirtCGIStdin(stdin_txt)\n"
"    sys.stderr = new_stderr\n"
"\n"
"    return save_env, si, so, sr\n"
"     \n"
"\n"
"def restore_nocgi(sav_env, si, so, sr):\n"
"\n"
"    osenv = os.environ\n"
"\n"
"    for k in osenv.keys():\n"
"        del osenv[k]\n"
"    for k in sav_env:\n"
"        osenv[k] = sav_env[k]\n"
"\n"
"    sys.stdout = si\n"
"    sys.stdin = so\n"
"    sys.stderr = sr\n"
"\n"
"\n"
"def call_string(base_uri,content,opts,params,lines):\n"
"	new_stdout = VirtCGIStdout();\n"
"	new_stderr = VirtCGIStdout();\n"
"\n"
"	sys.argv = [ base_uri ];\n"
"\n"
"	sav_env, si, so, sr = setup_cgi (opts, params, new_stdout, '')\n"
"	try:\n"
"	    eval (compile (content, base_uri, 'exec'), {})\n"
"	    restore_nocgi (sav_env, si, so, sr)\n"
"\n"
"            body=new_stdout.get_body ();\n"
"            hdr=new_stdout.get_headers ();\n"
"            err=new_stderr.get_headers () + new_stderr.get_body ();\n"
"	    return body, hdr, err\n"
"	except:\n"
"	    restore_nocgi (sav_env, si, so, sr)\n"
"            a,b,c = sys.exc_info ();\n"
"	    ex_text = \"\".join (traceback.format_exception (a,b,c));\n"
"            body=new_stdout.get_body ();\n"
"            hdr=new_stdout.get_headers ();\n"
"            err=new_stderr.get_headers () + new_stderr.get_body ();\n"
"            return body, hdr, err, ex_text\n"
"\n"
"\n"
"def call_file(base_uri,opts,params,lines):\n"
"	new_stdout = VirtCGIStdout();\n"
"	new_stderr = VirtCGIStdout();\n"
"\n"
"	sys.argv = [ base_uri ];\n"
"\n"
"	sav_env, si, so, sr = setup_cgi (opts, params, new_stdout, '')\n"
"	try:\n"
"	    execfile (base_uri, {});\n"
"	    restore_nocgi (sav_env, si, so, sr)\n"
"\n"
"            body=new_stdout.get_body ();\n"
"            hdr=new_stdout.get_headers ();\n"
"            err=new_stderr.get_headers () + new_stderr.get_body ();\n"
"	    return body, hdr, err\n"
"	except:\n"
"	    restore_nocgi (sav_env, si, so, sr)\n"
"            a,b,c = sys.exc_info ();\n"
"	    ex_text = \"\".join (traceback.format_exception (a,b,c));\n"
"            body=new_stdout.get_body ();\n"
"            hdr=new_stdout.get_headers ();\n"
"            err=new_stderr.get_headers () + new_stderr.get_body ();\n"
"            return body, hdr, err, ex_text\n"
"\n"
"# testting code\n"
"if __name__ == '__main__':\n"
"    if os.environ.has_key (\"__VIRT_CGI\") and os.environ[\"__VIRT_CGI\"] != '1' and not 12 == 11:\n"
"       print (\"xx\");\n"
"       \n"
"    a,b,c = call_file ('../lib/suite/admin/cgitest.py', { '__VIRT_CGI': '1' }, '', '');\n"
"    d = ''\n"
"    sys.stderr.write ('\\n['+a+']['+b+']['+c+']['+d+']\\n');\n"
;
