# Bang

## Introduce
"Bang!" is a faction-competition based game with the old-fashion western shoot-out theme.  
The Sheriff, the Outlaws, and the Renegade, each faction has thier own mission to achieve.  
With many kinds of characters and items, you can plot your own strategies with your teammate to hunt down your targets!  
Well, there's not only just strategy involved, you also need some luck to find the way to victory!  

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

- Using Dockerfile to create image 

    ```bash
    docker build -t bang . 
    ```
- Create container

    ```bash
    docker create -it --name bang-env bang
    ```

Play Bang in docker environment

- Run container

    ```
    docker start bang-env 
    ```
- Play bang

    ```
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
