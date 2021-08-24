#
# Makefile for key generation for U-Boot
#
# Copyright (C) 2015,2016 Baikal Electronics JSC.
# All rights reserved.
#
# Author:
#   Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
#
# This software may be modified and distributed under the terms
# of the BSD license.  See the LICENSE file for details.
#

NAME ?= baikal
PRIV := $(NAME).key
CERT := $(NAME).crt
PUB  := $(NAME).pub

all: $(PRIV) $(CERT) $(PUB)

clean:
	@rm -f $(PRIV) $(CERT) $(PUB)

$(PUB): $(PRIV)
	@openssl rsa -in $(PRIV) -pubout > $@

$(CERT): $(PRIV)
	@openssl req -batch -new -x509 -key $(PRIV) -out $@

$(PRIV):
	@openssl genpkey -algorithm RSA -out $@ -pkeyopt rsa_keygen_bits:2048 -pkeyopt rsa_keygen_pubexp:65537

.PHONY: all clean
