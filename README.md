# gymmy3
Jumping Jack Exercise Counter powered by CVDL

Project LICENSE to
Intel OpenVINO Toolkit 

Project Owner: Wilson Choo (yptheangel)
Contributors:
## Gymmy4AWS
Wilson Choo
Minemura Kazuki
Wan Arif WAn Abdul Hakim
Nahulanthran Sanjeev
## Gymmy3
Wilson Choo
Wan Arif WAn Abdul Hakim
Vincent Wong
Nahulanthran Sanjeev
## Gymmy2
Wilson Choo
Vincent Wong
Nahulanthran Sanjeev
Nahulanthran Shaundeep
## Gymmy
Wilson Choo

## To Compile and Run
### On Windows
`mkdir build && cd build`\
`"C:\Program Files (x86)\IntelSWTools\openvino\bin\setupvars.bat"`\
`cmake -G "Visual Studio 14 2015 Win64" ..`\
`cmake --build . --config Release`\
`cd intel64\Release`\
`human_pose_estimation_demo.exe -m ..\..\..\human-pose-estimation-0001.xml`\

### On Linux
`mkdir build && cd build`\
`source /opt/intel/openvino/bin/setupvars.sh`\
`cmake ..`\
`make`\
`./human_pose_estimation_demo -m ..\..\..\human-pose-estimation-0001.xml`



