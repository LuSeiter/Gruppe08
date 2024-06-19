################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
GPIO.obj: /Users/luisaseiter/Documents/GitHub/Gruppe08/Common_Classes/GPIO.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/PWM_Class_Test" --include_path="/Users/luisaseiter/Documents/KIT/MIT/2. Semester/ITAT/ITAT Praktikum/TivaWare_C_Series-2.1.4.178/utils" --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/Common_Classes" --include_path="/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

PWM.obj: /Users/luisaseiter/Documents/GitHub/Gruppe08/Common_Classes/PWM.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/PWM_Class_Test" --include_path="/Users/luisaseiter/Documents/KIT/MIT/2. Semester/ITAT/ITAT Praktikum/TivaWare_C_Series-2.1.4.178/utils" --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/Common_Classes" --include_path="/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

System.obj: /Users/luisaseiter/Documents/GitHub/Gruppe08/Common_Classes/System.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/PWM_Class_Test" --include_path="/Users/luisaseiter/Documents/KIT/MIT/2. Semester/ITAT/ITAT Praktikum/TivaWare_C_Series-2.1.4.178/utils" --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/Common_Classes" --include_path="/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/PWM_Class_Test" --include_path="/Users/luisaseiter/Documents/KIT/MIT/2. Semester/ITAT/ITAT Praktikum/TivaWare_C_Series-2.1.4.178/utils" --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/Common_Classes" --include_path="/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/PWM_Class_Test" --include_path="/Users/luisaseiter/Documents/KIT/MIT/2. Semester/ITAT/ITAT Praktikum/TivaWare_C_Series-2.1.4.178/utils" --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/Common_Classes" --include_path="/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

uartstdio.obj: /Users/luisaseiter/Documents/KIT/MIT/2.\ Semester/ITAT/ITAT\ Praktikum/TivaWare_C_Series-2.1.4.178/utils/uartstdio.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/PWM_Class_Test" --include_path="/Users/luisaseiter/Documents/KIT/MIT/2. Semester/ITAT/ITAT Praktikum/TivaWare_C_Series-2.1.4.178/utils" --include_path="/Users/luisaseiter/Documents/GitHub/Gruppe08/Common_Classes" --include_path="/Applications/ti/ccs1271/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


