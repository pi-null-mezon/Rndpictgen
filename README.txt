Application was designed for automatical random picture generation.

A .bat script example for run utility at MS Windows cmd.exe 
   mkdir output
   for /l %%g in (1,1,25) do randpicture -x1024 -y1024 -d23 -r5 -t7 -e11 -b -i -u -s53 -n11 -ooutput/Sample%%g.png
   for /l %%g in (1,1,25) do randpicture -x1024 -y1024 -d31 -r11 -t5 -e7 -g -b -i -u -s53 -n11 -ooutput/SampleGS%%g.png
