cpp_utils_all_files               := $(cpp_utils_path)/compiler.hpp \
                                     $(cpp_utils_path)/const_string.hpp \
					                 $(cpp_utils_path)/diagnostics.hpp \
					                 $(cpp_utils_path)/math.hpp \
					                 $(cpp_utils_path)/meta.hpp \
					                 $(cpp_utils_path)/multithreading.hpp \
					                 $(cpp_utils_path)/pointer.hpp \
									 $(cpp_utils_path)/print.hpp \
					                 $(cpp_utils_path)/windows_console.hpp \
					                 $(cpp_utils_path)/windows_impl.hpp
cpp_utils_windows_impl_win7_args := -lshlwapi -DCPP_UTILS_WINDOWS_IMPL_WIN7_FIX