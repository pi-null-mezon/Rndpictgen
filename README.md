# randpicture
Application was designed for automatical random picture generation

[Show me a sample](https://cloud.githubusercontent.com/assets/5991537/9684822/cf880dd4-5322-11e5-80ad-f7f20e8a7fc5.png)

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
