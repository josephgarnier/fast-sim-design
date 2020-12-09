#=============================================================================
# Copyright 2005-2011 Kitware, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
# * Neither the name of Kitware, Inc. nor the names of its
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

######################################
#
#       Macros for building Qt files
#
######################################

include(CMakeParseArguments)

# qt6_add_resources(target resourcename ...
# or
# qt6_add_resources(outfiles inputfile ... )

function(QT6_ADD_RESOURCES_CUSTOM outfiles)
	if (TARGET ${outfiles})
		cmake_parse_arguments(arg "" "OUTPUT_TARGETS" "" ${ARGN})
		_qt_internal_process_resource(${ARGV})
		if (arg_OUTPUT_TARGETS)
			set(${arg_OUTPUT_TARGETS} ${${arg_OUTPUT_TARGETS}} PARENT_SCOPE)
		endif()
	else()
		set(options)
		set(oneValueArgs)
		set(multiValueArgs OPTIONS)

		cmake_parse_arguments(_RCC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

		set(rcc_files ${_RCC_UNPARSED_ARGUMENTS})
		set(rcc_options ${_RCC_OPTIONS})

		if("${rcc_options}" MATCHES "-binary")
			message(WARNING "Use qt6_add_binary_resources for binary option")
		endif()

		foreach(it ${rcc_files})
			get_filename_component(outfilename ${it} NAME_WE)
			get_filename_component(infile ${it} ABSOLUTE)
			set(outfile ${CMAKE_CURRENT_BINARY_DIR}/src/qrc_${outfilename}.cpp)

			_qt6_parse_qrc_file(${infile} _out_depends _rc_depends)
			set_source_files_properties(${infile} PROPERTIES SKIP_AUTORCC ON)

			add_custom_command(OUTPUT ${outfile}
				COMMAND ${QT_CMAKE_EXPORT_NAMESPACE}::rcc
				ARGS ${rcc_options} --name ${outfilename} --output ${outfile} ${infile}
				MAIN_DEPENDENCY ${infile}
				DEPENDS ${_rc_depends} "${_out_depends}" ${QT_CMAKE_EXPORT_NAMESPACE}::rcc
				VERBATIM)
			set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOMOC ON)
			set_source_files_properties(${outfile} PROPERTIES SKIP_AUTOUIC ON)
			list(APPEND ${outfiles} ${outfile})
		endforeach()
		set(${outfiles} ${${outfiles}} PARENT_SCOPE)
	endif()
endfunction()