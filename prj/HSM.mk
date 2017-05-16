##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=HSM
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/Tedi/Desktop/New_HSM/HSM/prj
ProjectPath            :=C:/Users/Tedi/Desktop/New_HSM/HSM/prj
IntermediateDirectory  :=./../obj
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Tedi
Date                   :=16/05/2017
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/TDM-GCC-32/bin/g++.exe
SharedObjectLinkerName :=C:/TDM-GCC-32/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=./../bin/Debug/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="HSM.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-32/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../src 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)Unit 
ArLibs                 :=  "libUnit.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/TDM-GCC-32/bin/ar.exe rcu
CXX      := C:/TDM-GCC-32/bin/g++.exe
CC       := C:/TDM-GCC-32/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-32/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_src_main.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_console.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_hsm.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_newSys.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./../obj"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./../obj"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_src_main.c$(ObjectSuffix): ../src/main.c $(IntermediateDirectory)/up_src_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Tedi/Desktop/New_HSM/HSM/src/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_main.c$(DependSuffix): ../src/main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_main.c$(DependSuffix) -MM ../src/main.c

$(IntermediateDirectory)/up_src_main.c$(PreprocessSuffix): ../src/main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_main.c$(PreprocessSuffix) ../src/main.c

$(IntermediateDirectory)/up_src_console.c$(ObjectSuffix): ../src/console.c $(IntermediateDirectory)/up_src_console.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Tedi/Desktop/New_HSM/HSM/src/console.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_console.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_console.c$(DependSuffix): ../src/console.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_console.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_console.c$(DependSuffix) -MM ../src/console.c

$(IntermediateDirectory)/up_src_console.c$(PreprocessSuffix): ../src/console.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_console.c$(PreprocessSuffix) ../src/console.c

$(IntermediateDirectory)/up_src_hsm.c$(ObjectSuffix): ../src/hsm.c $(IntermediateDirectory)/up_src_hsm.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Tedi/Desktop/New_HSM/HSM/src/hsm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_hsm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_hsm.c$(DependSuffix): ../src/hsm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_hsm.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_hsm.c$(DependSuffix) -MM ../src/hsm.c

$(IntermediateDirectory)/up_src_hsm.c$(PreprocessSuffix): ../src/hsm.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_hsm.c$(PreprocessSuffix) ../src/hsm.c

$(IntermediateDirectory)/up_src_newSys.c$(ObjectSuffix): ../src/newSys.c $(IntermediateDirectory)/up_src_newSys.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Tedi/Desktop/New_HSM/HSM/src/newSys.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_newSys.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_newSys.c$(DependSuffix): ../src/newSys.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_newSys.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_newSys.c$(DependSuffix) -MM ../src/newSys.c

$(IntermediateDirectory)/up_src_newSys.c$(PreprocessSuffix): ../src/newSys.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_newSys.c$(PreprocessSuffix) ../src/newSys.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./../obj/


