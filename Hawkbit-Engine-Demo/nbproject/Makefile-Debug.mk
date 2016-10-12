#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/GameplayScene.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Llib -lhawkbit-engine-lib `pkg-config --libs glew` `pkg-config --libs sdl2` `pkg-config --libs assimp` `pkg-config --libs sndfile` `pkg-config --libs openal` `pkg-config --libs libjpeg` -lSOIL  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hawkbit-engine-demo

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hawkbit-engine-demo: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/hawkbit-engine-demo ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/GameplayScene.o: GameplayScene.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../Hawkbit-Engine-Lib `pkg-config --cflags glew` `pkg-config --cflags sdl2` `pkg-config --cflags assimp` `pkg-config --cflags sndfile` `pkg-config --cflags openal` `pkg-config --cflags libjpeg` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GameplayScene.o GameplayScene.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../Hawkbit-Engine-Lib `pkg-config --cflags glew` `pkg-config --cflags sdl2` `pkg-config --cflags assimp` `pkg-config --cflags sndfile` `pkg-config --cflags openal` `pkg-config --cflags libjpeg` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
