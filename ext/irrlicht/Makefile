
	
this:
	@if [ ! -d "$(CURDIR)/ext/irrlicht/trunk" ]; then \
		svn co http://svn.code.sf.net/p/irrlicht/code/trunk trunk; \
		$(MAKE) -C $(CURDIR)/trunk/source/Irrlicht; \
	fi;

debian:
	sudo apt-get install libglu1-mesa-dev libgl1-mesa-dev libX11-dev libx11-xcb-dev libxcb-glx0-dev libxcb1-dev libxxf86vm-dev libXext-dev libxdmcp-dev libxau-dev libxdamage-dev libxfixes-dev libdrm-dev 