# Rndpictgen
Application was designed for automatical random picture generation.
Generates pictures with user defined quantities of geometric primitives (triangles, rects, ellipses) and ASCII symbols. Automatically scales, rotates and translates primitives over picture area. Distorts picture by adding noise and illumination gradient. To read a list of all options send to cmd -h option.

Sample of the output:

![Picture](https://github.com/pi-null-mezon/randpicture/blob/master/Resources/Sample.png)


Script example for run utility at MS Windows cmd.exe (replace % by %% if run in batch file): 
```cmd
mkdir output
for /l %g in (1,1,10) do randpicture -x1024 -y1024 -d23 -r5 -t7 -e11 -b -i -u -s53 -n11 -ooutput/Sample%g.png
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
