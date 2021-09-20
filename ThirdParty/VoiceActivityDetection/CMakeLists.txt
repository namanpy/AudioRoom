cmake_minimum_required(VERSION 3.10)

# set the project name
project(webrtcvad)
include_directories(${CMAKE_CURRENT_LIST_DIR})

add_definitions(-DWEBRTC_POSIX)

# add the executable
add_library(webrtcvad STATIC
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/auto_correlation.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/auto_corr_to_refl_coef.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/complex_bit_reverse.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/complex_fft.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/copy_set_operations.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/cross_correlation.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/division_operations.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/dot_product_with_scale.cc
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/dot_product_with_scale.h
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/downsample_fast.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/energy.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/filter_ar.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/filter_ar_fast_q12.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/filter_ma_fast_q12.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/get_hanning_window.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/get_scaling_square.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/ilbc_specific_functions.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/levinson_durbin.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/lpc_to_refl_coef.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/min_max_operations.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/randomization_functions.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/real_fft.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/refl_coef_to_lpc.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/resample.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/resample_48khz.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/resample_by_2.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/resample_by_2_internal.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/resample_fractional.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/splitting_filter.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/spl_init.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/spl_inl.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/spl_sqrt.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/sqrt_of_one_minus_x_squared.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/signal_processing/vector_scaling_operations.c
${CMAKE_CURRENT_LIST_DIR}/rtc_base/checks.cc
${CMAKE_CURRENT_LIST_DIR}/common_audio/vad/vad_filterbank.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/vad/vad_gmm.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/vad/vad_sp.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/vad/vad_core.c
${CMAKE_CURRENT_LIST_DIR}/common_audio/vad/vad.cc
${CMAKE_CURRENT_LIST_DIR}/common_audio/vad/webrtc_vad.c 

)

