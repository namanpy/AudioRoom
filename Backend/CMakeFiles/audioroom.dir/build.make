# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/Desktop/AudioRoom

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/Desktop/AudioRoom

# Include any dependencies generated for this target.
include CMakeFiles/audioroom.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/audioroom.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/audioroom.dir/flags.make

CMakeFiles/audioroom.dir/Certificate.cpp.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/Certificate.cpp.o: Certificate.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/audioroom.dir/Certificate.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/Certificate.cpp.o -c /root/Desktop/AudioRoom/Certificate.cpp

CMakeFiles/audioroom.dir/Certificate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/Certificate.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/Certificate.cpp > CMakeFiles/audioroom.dir/Certificate.cpp.i

CMakeFiles/audioroom.dir/Certificate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/Certificate.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/Certificate.cpp -o CMakeFiles/audioroom.dir/Certificate.cpp.s

CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.o: WebRTC/RTPPacket.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.o -c /root/Desktop/AudioRoom/WebRTC/RTPPacket.cc

CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/WebRTC/RTPPacket.cc > CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.i

CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/WebRTC/RTPPacket.cc -o CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.s

CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.o: WebRTC/PeerConnection.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.o -c /root/Desktop/AudioRoom/WebRTC/PeerConnection.cc

CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/WebRTC/PeerConnection.cc > CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.i

CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/WebRTC/PeerConnection.cc -o CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.s

CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.o: WebRTC/Sdp.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.o -c /root/Desktop/AudioRoom/WebRTC/Sdp.cc

CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/WebRTC/Sdp.cc > CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.i

CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/WebRTC/Sdp.cc -o CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.s

CMakeFiles/audioroom.dir/WebRTC/Ice.cc.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/WebRTC/Ice.cc.o: WebRTC/Ice.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/audioroom.dir/WebRTC/Ice.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/WebRTC/Ice.cc.o -c /root/Desktop/AudioRoom/WebRTC/Ice.cc

CMakeFiles/audioroom.dir/WebRTC/Ice.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/WebRTC/Ice.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/WebRTC/Ice.cc > CMakeFiles/audioroom.dir/WebRTC/Ice.cc.i

CMakeFiles/audioroom.dir/WebRTC/Ice.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/WebRTC/Ice.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/WebRTC/Ice.cc -o CMakeFiles/audioroom.dir/WebRTC/Ice.cc.s

CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.o: WebRTC/RtpHandler.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.o -c /root/Desktop/AudioRoom/WebRTC/RtpHandler.cc

CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/WebRTC/RtpHandler.cc > CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.i

CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/WebRTC/RtpHandler.cc -o CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.s

CMakeFiles/audioroom.dir/SignalServer.cpp.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/SignalServer.cpp.o: SignalServer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/audioroom.dir/SignalServer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/SignalServer.cpp.o -c /root/Desktop/AudioRoom/SignalServer.cpp

CMakeFiles/audioroom.dir/SignalServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/SignalServer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/SignalServer.cpp > CMakeFiles/audioroom.dir/SignalServer.cpp.i

CMakeFiles/audioroom.dir/SignalServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/SignalServer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/SignalServer.cpp -o CMakeFiles/audioroom.dir/SignalServer.cpp.s

CMakeFiles/audioroom.dir/main.cpp.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/audioroom.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/main.cpp.o -c /root/Desktop/AudioRoom/main.cpp

CMakeFiles/audioroom.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/main.cpp > CMakeFiles/audioroom.dir/main.cpp.i

CMakeFiles/audioroom.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/main.cpp -o CMakeFiles/audioroom.dir/main.cpp.s

CMakeFiles/audioroom.dir/EventEmitter.cpp.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/EventEmitter.cpp.o: EventEmitter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/audioroom.dir/EventEmitter.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/EventEmitter.cpp.o -c /root/Desktop/AudioRoom/EventEmitter.cpp

CMakeFiles/audioroom.dir/EventEmitter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/EventEmitter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/EventEmitter.cpp > CMakeFiles/audioroom.dir/EventEmitter.cpp.i

CMakeFiles/audioroom.dir/EventEmitter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/EventEmitter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/EventEmitter.cpp -o CMakeFiles/audioroom.dir/EventEmitter.cpp.s

CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.o: CMakeFiles/audioroom.dir/flags.make
CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.o: CallServer/CallServer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.o -c /root/Desktop/AudioRoom/CallServer/CallServer.cpp

CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Desktop/AudioRoom/CallServer/CallServer.cpp > CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.i

CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Desktop/AudioRoom/CallServer/CallServer.cpp -o CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.s

# Object files for target audioroom
audioroom_OBJECTS = \
"CMakeFiles/audioroom.dir/Certificate.cpp.o" \
"CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.o" \
"CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.o" \
"CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.o" \
"CMakeFiles/audioroom.dir/WebRTC/Ice.cc.o" \
"CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.o" \
"CMakeFiles/audioroom.dir/SignalServer.cpp.o" \
"CMakeFiles/audioroom.dir/main.cpp.o" \
"CMakeFiles/audioroom.dir/EventEmitter.cpp.o" \
"CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.o"

# External object files for target audioroom
audioroom_EXTERNAL_OBJECTS =

audioroom: CMakeFiles/audioroom.dir/Certificate.cpp.o
audioroom: CMakeFiles/audioroom.dir/WebRTC/RTPPacket.cc.o
audioroom: CMakeFiles/audioroom.dir/WebRTC/PeerConnection.cc.o
audioroom: CMakeFiles/audioroom.dir/WebRTC/Sdp.cc.o
audioroom: CMakeFiles/audioroom.dir/WebRTC/Ice.cc.o
audioroom: CMakeFiles/audioroom.dir/WebRTC/RtpHandler.cc.o
audioroom: CMakeFiles/audioroom.dir/SignalServer.cpp.o
audioroom: CMakeFiles/audioroom.dir/main.cpp.o
audioroom: CMakeFiles/audioroom.dir/EventEmitter.cpp.o
audioroom: CMakeFiles/audioroom.dir/CallServer/CallServer.cpp.o
audioroom: CMakeFiles/audioroom.dir/build.make
audioroom: libwebrtcvad.a
audioroom: CMakeFiles/audioroom.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/Desktop/AudioRoom/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable audioroom"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/audioroom.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/audioroom.dir/build: audioroom

.PHONY : CMakeFiles/audioroom.dir/build

CMakeFiles/audioroom.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/audioroom.dir/cmake_clean.cmake
.PHONY : CMakeFiles/audioroom.dir/clean

CMakeFiles/audioroom.dir/depend:
	cd /root/Desktop/AudioRoom && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/Desktop/AudioRoom /root/Desktop/AudioRoom /root/Desktop/AudioRoom /root/Desktop/AudioRoom /root/Desktop/AudioRoom/CMakeFiles/audioroom.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/audioroom.dir/depend

