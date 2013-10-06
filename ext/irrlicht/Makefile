
	
this:
	@if [ ! -d "$(CURDIR)/ext/irrlicht/trunk" ]; then \
		svn co http://svn.code.sf.net/p/irrlicht/code/trunk trunk; \
		$(MAKE) -C $(CURDIR)/trunk/source/Irrlicht; \
	fi;