SET PATH=C:\Keil_v5\ARM\ARMCLANG\Bin;C:\Program Files (x86)\VMware\VMware Workstation\bin\;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\bin;C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\libnvvp;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;C:\Program Files\NVIDIA Corporation\Nsight Compute 2022.1.1\;C:\ufgovApp\Camera\SZLT\;D:\Program\Anaconda3;D:\Program\Anaconda3\Scripts;C:\Program Files\Git\cmd;D:\Program\Anaconda3\envs\pytorch\Scripts;D:\Program\Anaconda3\envs\pytorch\Lib\site-packages\PySide2\plugins\platforms;D:\Program\Anaconda3\envs\pytorch\Lib\site-packages\PySide2;C:\Program Files\dotnet\;C:\Program Files\Microsoft SQL Server\150\Tools\Binn\;C:\Users\Administrator\.platformio\penv\Scripts;;C:\Program Files\Microsoft VS Code\bin;C:\Users\Administrator\scoop\shims;C:\Users\Administrator\AppData\Local\Microsoft\WindowsApps;C:\Users\Administrator\AppData\Local\GitHubDesktop\bin;d:\Program\JetBrains\PyCharm 2022.1.2\bin;C:\Users\Administrator\.dotnet\tools
SET CPU_TYPE=STM32L451VETx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\startup_stm32l451xx._ac"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\system_stm32l4xx.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_adc.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_adc_ex.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_can.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_comp.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_cortex.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_crc.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_crc_ex.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_cryp.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_cryp_ex.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_dac.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_dac_ex.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_dcmi.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\stm32l4xx_hal_dfsdm.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\Objects\STM32L4xx.lnp"
"C:\Keil_v5\ARM\ARMCLANG\Bin\fromelf.exe" ".\Objects\STM32L4xx.axf" --i32combined --output ".\Objects\STM32L4xx.hex"
