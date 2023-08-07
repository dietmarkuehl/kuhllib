#  Makefile                                                      -*-makefile-*-
#  ----------------------------------------------------------------------------
#   Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
#
#   Permission is hereby granted, free of charge, to any person
#   obtaining a copy of this software and associated documentation
#   files (the "Software"), to deal in the Software without restriction,
#   including without limitation the rights to use, copy, modify,
#   merge, publish, distribute, sublicense, and/or sell copies of
#   the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be
#   included in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
#   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#   OTHER DEALINGS IN THE SOFTWARE.
#  ----------------------------------------------------------------------------

FILES = ca_keypair.pem ca_cert.pem server_keypair.pem server_csr.pem server_cert.pem

.PHONY: default clean distclean

default: $(FILES)

ca_keypair.pem ca_cert.pem:
	openssl req -newkey ED448 -x509 -subj "/CN=Root CA" -addext "basicConstraints=critical,CA:TRUE" -days 3650 -noenc -keyout ca_keypair.pem -out ca_cert.pem

server_keypair.pem server_csr.pem:
	openssl req -newkey ED448 -subj "/CN=localhost" -addext "basicConstraints=critical,CA:FALSE" -noenc -keyout server_keypair.pem -out server_csr.pem

server_cert.pem: server_csr.pem ca_cert.pem ca_keypair.pem
	openssl x509 -req -in server_csr.pem -copy_extensions copyall -CA ca_cert.pem -CAkey ca_keypair.pem -days 3650 -out server_cert.pem

clean:
	$(RM) mkerr olderr *~

distclean: clean
	$(RM) $(FILES)
