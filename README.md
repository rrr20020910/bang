# Bang

## Introduce
<!-- TODO: introduce bang game -->

## How to Play
If you want to play Bang! with music please make sure you install sox
```bash
sudo apt update
sudo apt install sox
sudo apt-get install libsox-fmt-all
```

And then run
``` bash
sox --version # make sure you successfully install sox
```

For original version
```bash
./bin/main 
```

If you want to enjoy music in game
```bash
(play ./assets/background.mp3 -q repeat 32) & ./bin/main
```

In Docker version, to initialize docker environment
```
docker build -t bang . 
docker create -it --name bang-env bang
```

Play Bang in docker environment
```
docker start bang-env
docker exec -it bang-env make;./bin/main;
```
> note: the music version may not work because your audio device is not connect to docker container.

## Other Info
This is final project in NTNU computer programming II 2022
- Instructor: 
    - 紀博文 Po-Wen Chi

- Team Members:
    - 41047026S 黃文雄
    - 41047019S 林柏佑
    - 41047035S 林昕鋭