# randpicture
Application was designed for automatical random picture generation.
Generates pictures with user defined quantities of geometric primitives (triangles, rects, ellipses) and ASCII symbols. Automatically scales, rotates and translates primitives over picture area. Distorts picture by adding noise and illumination gradient. To read a list of all options send to cmd -h option.
Samples of output pictures:    
[Sample1](https://cloud.githubusercontent.com/assets/5991537/9684822/cf880dd4-5322-11e5-80ad-f7f20e8a7fc5.png)
[Sample2](https://cloud.githubusercontent.com/assets/5991537/11300069/ffca2584-8f9c-11e5-8bfe-6f2e3fbb7a5d.png)
[Sample3](https://cloud.githubusercontent.com/assets/5991537/11300135/94d05252-8f9d-11e5-98c6-ea206bdc12c9.png)

Script example for run utility at MS Windows cmd.exe: 
```cmd
mkdir output
for /l %%g in (1,1,10) do randpicture -x1024 -y1024 -d23 -r5 -t7 -e11 -b -i -u -s53 -n11 -ooutput/Sample%%g.png
```

And bash script example:
```bash
mkdir output
for i in {1..5}; do
./randpicture -x1024 -y1024 -d23 -r5 -t7 -e11 -b -i -u -s53 -n11 -ooutput/Sample$(($(($i * 2)) - 1)).png &
./randpicture -x1024 -y1024 -d23 -r5 -t7 -e11 -b -i -u -s53 -n11 -ooutput/Sample$(($i * 2)).png &
wait
done
```
