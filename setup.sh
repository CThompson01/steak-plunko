# Print to stderr then exit
function die {
	echo -e "$@" '\nAbort.' 1>&2
	exit 1
}

# Check for required libraries on Ubuntu
function ubuntuLibs {
	declare -r -a required_libs=("libasound2-dev" "libx11-dev" "libxrandr-dev" "libxi-dev" "libgl1-mesa-dev"
		"libglu1-mesa-dev" "libxcursor-dev" "libxinerama-dev" "libwayland-dev" "libxkbcommon-dev")
	declare needed_libs=""

	# Search for required_libs
	for lib in "${required_libs[@]}"
	do
		if dpkg -s $lib &> /dev/null; then
			echo "$lib found"
		else
			echo "$lib not found..."
			needed_libs="$needed_libs $lib"
		fi
	done

	# Install all libraries that were not found
	if [ "$needed_libs" = "" ]; then
		echo 'All required libraries are already installed!'
	else
		echo "Installing ${needed_libs:1}..."
		sudo apt-get install $needed_libs || die "Failed to install $needed_libs."
	fi
}

# Check for build-essential
echo 'Running generic setup portion...'
if dpkg -s build-essential &> /dev/null; then
	echo 'build-essential found'
else
	echo 'Installing build-essential...'
	sudo apt-get install build-essential || die "Failed to install build-essentials."
fi

# Get the version of Linux running
if [ -f /etc/os-release ]; then
	. /etc/os-release
	declare -g SETUP=$NAME
else
	die 'Unable to determine OS Version'
fi

# Run the setup depending on version
if [ "$SETUP" = "Ubuntu" ]; then
	echo 'Running setup for Ubuntu...'
	ubuntuLibs
else
	die 'Unable to determine OS Version'
fi

# Setup complete
echo 'Setup completed successfully!'
exit 0