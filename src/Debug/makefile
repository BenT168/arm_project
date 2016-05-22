################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
<<<<<<< HEAD
all: ARMProject

# Tool invocations
ARMProject: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc  -o "ARMProject" $(OBJS) $(USER_OBJS) $(LIBS)
=======
all: ARM

# Tool invocations
ARM: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cygwin C Linker'
	gcc  -o "ARM" $(OBJS) $(USER_OBJS) $(LIBS)
>>>>>>> d92123bcbc824b444712f5cdd0ff06e09b9d2171
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
<<<<<<< HEAD
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) ARMProject
=======
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) ARM
>>>>>>> d92123bcbc824b444712f5cdd0ff06e09b9d2171
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
