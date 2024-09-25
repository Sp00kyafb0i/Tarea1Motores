
#include <rusty_audio.h>
#include <cmath>
#include <limits>
#include <iostream>
#include <numbers>
#include <nothofagus.h>
#include <ctime>
using namespace std;



//Se debe spawnear una bellota que contenga al jugador y mantenerla en el centro.
//Se deben spawnear varias bellotas que representen los obstaculos del camino, estas son las que se mueven ya que el jugador se mantiene en el centro de la pantalla.
//Cada obstaculo debe tener una textura asociada y un tamaño (Todos seran rectangulares para simplificar el calculo de colisiones).
//Las colisiones deben calcularse usando las esquinas de cada bellota revisando si alguna esquina del jugador esta dentro de alguna bellota ajena.
//Para spawnear al hombre de las nieves se debe hacer un random cada tick despues de x tiempo, este enemigo debe acercarse cada vez mas a la bellota del jugador.
//Las colisiones deben triggerear uno de 4 efectos dependiendo del objeto con el que se choca: Aumentar estilo, bajar velocidad a 0, perder o ganar.


int main()
{
    float acceleration = 0.0001f;//Constante de cambio de velocidad
    float speedx = 0;
    float speedy = 0;
    float speed = 0;//Velocidad de los objetos relativo al jugador
    int style = 0;//Puntos de estilo, deberian aumentar al colisionar con una rampa
    float dist = 0;//Distancia que queda hasta la meta, deberia ser calculado una vez la meta spawnee
    float dtTime = 0;
    time_t t0;
    time(&t0);
    bool downKeyPressed = false;
    bool leftKeyPressed = false;
    bool rightKeyPressed = false;
    bool upKeyPressed = false;
    float minSpeed = 0.02;
    int spriteSize = 8;
    float width = 600;
    float height = 300;
    float x0 = width/2;
    float y0 = height/2;
    //Esquinas de el jugador:
    float leftX = x0 - spriteSize / 2;
    float downY = y0 - spriteSize / 2;
    float rightX = x0 + spriteSize / 2;
    float upY = y0 + spriteSize / 2;
    int cooldown = 0;
    int maxCd = 90;
    bool monster = false;
    int N = 120;
    int monsterTime = 120 + rand() % N;
    float monsterSpeed = 0.07;
    srand(time(0));
    float distancia = 50.0;
    float metaX = 36;
    float metaY = 2;
    bool caught = false;
    int finalTime = 0;



    Nothofagus::ScreenSize screenSize{ width, height };
    Nothofagus::Canvas canvas(screenSize, "Tarea 1", { 1.0, 1.0, 1.0 }, 3);



    Nothofagus::ColorPallete pallete0{//Paleta del jugador
        {0.0, 0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0, 1.0}
    };

    Nothofagus::ColorPallete pallete1{//Paleta de obstaculos
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 1.0},
        {88/255, 57/255, 39/255, 1.0}
    };

    Nothofagus::ColorPallete pallete2{//Paleta de rampas
        {0.0, 0.0, 1.0, 1.0},
        {1.0, 1.0, 0.0, 1.0}
    };

    Nothofagus::ColorPallete pallete3{//Paleta de monstruo
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

    Nothofagus::ColorPallete pallete4{//Paleta de meta
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 1.0},
        {1.0, 1.0, 1.0, 1.0}
    };

    Nothofagus::Texture texture0({ spriteSize, spriteSize }, { 0.5, 0.5, 0.5, 1.0 });//Sprite para el jugador
    texture0.setPallete(pallete0)
        .setPixels(
            {
                   0,0,1,1,1,1,0,0,
                   0,1,1,0,0,1,1,0,
                   0,1,0,1,1,0,1,0,
                   0,1,0,1,1,0,1,0,
                   0,1,1,1,1,1,1,0,
                   0,1,1,1,1,1,1,0,
                   0,0,1,1,1,1,0,0,
                   0,0,0,1,1,0,0,0
            }
        );

    Nothofagus::Texture texture1({ spriteSize, spriteSize }, { 0.5, 0.5, 0.5, 1.0 });//Sprite para los obstaculos
    texture1.setPallete(pallete1)
        .setPixels(
            {
                    0,0,0,1,1,0,0,0,
                    0,0,1,1,1,1,0,0,
                    0,0,1,1,1,1,0,0,
                    0,1,1,1,1,1,1,0,
                    0,1,1,1,1,1,1,0,
                    0,0,0,2,2,0,0,0,
                    0,0,0,2,2,0,0,0,
                    0,0,0,2,2,0,0,0
            }
        );

    Nothofagus::Texture texture2({ spriteSize, spriteSize }, { 0.5, 0.5, 0.5, 1.0 });//Sprite para las rampas
    texture2.setPallete(pallete2)
        .setPixels(
            {
                    1,0,0,0,0,0,0,1,
                    0,1,0,0,0,0,1,0,
                    0,0,1,0,0,1,0,0,
                    0,0,0,1,1,0,0,0,
                    1,0,0,0,0,0,0,1,
                    0,1,0,0,0,0,1,0,
                    0,0,1,0,0,1,0,0,
                    0,0,0,1,1,0,0,0
            }
        );

    Nothofagus::Texture texture3({ spriteSize, spriteSize }, { 0.5, 0.5, 0.5, 1.0 });//Sprite para el monstruo
    texture3.setPallete(pallete3)
        .setPixels(
            {
                   0,0,1,1,1,1,0,0,
                   0,1,1,0,0,1,1,0,
                   0,1,0,1,1,0,1,0,
                   0,1,1,0,0,1,1,0,
                   0,1,1,1,1,1,1,0,
                   0,0,1,1,1,1,0,0,
                   0,0,1,0,0,1,0,0,
                   0,0,1,1,1,1,0,0
            }
        );

    Nothofagus::Texture texture4({ metaX, metaY }, { 0.5, 0.5, 0.5, 1.0 });//Sprite para la meta
    texture4.setPallete(pallete4)
        .setPixels(
            {
                    2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,
                    1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2
            }
        );

    Nothofagus::TextureId textureId0 = canvas.addTexture(texture0);
    Nothofagus::BellotaId bellotaId0 = canvas.addBellota({ {{x0, y0}}, textureId0 });

    Nothofagus::TextureId textureId1 = canvas.addTexture(texture1);
    Nothofagus::TextureId textureId2 = canvas.addTexture(texture2);
    Nothofagus::TextureId textureId3 = canvas.addTexture(texture3);
    Nothofagus::TextureId textureId4 = canvas.addTexture(texture4);


    Nothofagus::BellotaId bellotaId3 = canvas.addBellota({ {{x0 + (rand() % 100), y0 + 75 + (rand() % 30)}}, textureId3 });
    Nothofagus::BellotaId bellotaId4 = canvas.addBellota({ {{x0 + (rand() % 30), y0 - 1500 - (rand() % 100)}}, textureId4 });
    Nothofagus::Bellota& bellota3 = canvas.bellota(bellotaId3);
    Nothofagus::Bellota& bellota4 = canvas.bellota(bellotaId4);
    bellota3.visible() = false;
    constexpr int objCnt = 15;
 
    Nothofagus::BellotaId listaObstaculos[objCnt];
    Nothofagus::BellotaId listaRampas[objCnt];
    int cnt = 0;
    int rangex = height;
    int rangey = width*3;
    while (cnt < objCnt) {
        listaObstaculos[cnt] = canvas.addBellota({ {{x0 + rand() % rangex - rangex /2, y0 + rand() % rangey - rangey /2}}, textureId1 });
        listaRampas[cnt] = canvas.addBellota({ {{x0 + rand() % rangex - rangex /2, y0 + rand() % rangey - rangey /2}}, textureId2 });
        cnt++;
    }
    
    

    constexpr unsigned int sampleRate = 48000;
    constexpr unsigned int channels = 2;

    RustyAudio::Builder soundBuilder;//Sonido de choque
    soundBuilder.appendSinusoids({
        {80, 0.1, 400},
        {80, 0.1, 300},
        {80, 0.1, 200},
        {280, 0.1, 100},
        {300, 0.1, 80}//Disminucion de frecuencia "lenta"
    });

    RustyAudio::Builder soundBuilder2;//Sonido de aumento de velocidad
    soundBuilder2.appendSinusoids({
        {100, 0.1, 800},
        {100, 0.1, 700},
        {100, 0.1, 600},
        {100, 0.1, 500},
        {100, 0.1, 600},
        {100, 0.1, 500},
        {100, 0.1, 600},//Aumento de frecuencia "erratico"
    });

    RustyAudio::Builder soundBuilder3;//Sonido de Monstruo
    soundBuilder3.appendSinusoids({
        {200, 0.1, 400},
        {200, 0.1, 300},
        {200, 0.1, 200},
        });

    RustyAudio::Builder soundBuilder4;//Sonido de Meta
    soundBuilder4.appendSinusoids({
        {200, 0.1, 500},
        {100, 0.1, 800},
        {100, 0.1, 650},
        {200, 0.1, 550},
        {100, 0.1, 650},

        });

    RustyAudio::Buffer soundBuffer = soundBuilder.generate(sampleRate, channels);
    RustyAudio::Buffer soundBuffer2 = soundBuilder2.generate(sampleRate, channels);
    RustyAudio::Buffer soundBuffer3 = soundBuilder3.generate(sampleRate, channels);
    RustyAudio::Buffer soundBuffer4 = soundBuilder4.generate(sampleRate, channels);

    RustyAudio::Player soundPlayer;
    RustyAudio::Player soundPlayer2;
    RustyAudio::Player soundPlayer3;
    RustyAudio::Player soundPlayer4;
    soundPlayer.init(soundBuffer);
    soundPlayer2.init(soundBuffer2);
    soundPlayer3.init(soundBuffer3);
    soundPlayer4.init(soundBuffer4);

  
    

    auto update = [&](float dt) {
        dtTime += dt;
        int cnt2 = 0;
        cooldown--;
        Nothofagus::Bellota& bellota0 = canvas.bellota(bellotaId0);


        if (!caught) {
            //Loop para obstaculos
            while (cnt2 < objCnt) {
                Nothofagus::Bellota& bellota = canvas.bellota(listaObstaculos[cnt2]);
                bellota.transform().location().x += -speedx * dt;
                bellota.transform().location().y += speedy * dt;
                float x = bellota.transform().location().x;
                float y = bellota.transform().location().y;
                if (y > height) {
                    bellota.transform().location().y -= height;
                    bellota.transform().location().x += rand() % 30 - 15;
                }
                if (x > width) {
                    bellota.transform().location().x -= width;
                }
                if (x < 0) {
                    bellota.transform().location().x += width;
                }
                //Manejo de colision
                if (leftX > x - spriteSize / 2 && leftX < x + spriteSize / 2 && downY > y - spriteSize / 2 && downY < y + spriteSize / 2 ||
                    leftX > x - spriteSize / 2 && leftX < x + spriteSize / 2 && upY > y - spriteSize / 2 && upY < y + spriteSize / 2 ||
                    rightX > x - spriteSize / 2 && rightX < x + spriteSize / 2 && downY > y - spriteSize / 2 && downY < y + spriteSize / 2 ||
                    rightX > x - spriteSize / 2 && rightX < x + spriteSize / 2 && upY > y - spriteSize / 2 && upY < y + spriteSize / 2) {
                    if (cooldown <= 0) {
                        soundPlayer.play();
                        cooldown = maxCd;
                        speedy = minSpeed;
                        if (speedx >= 0) {
                            speedx = minSpeed;
                        }
                        else {
                            speedx = -minSpeed;
                        }
                        style -= 20;
                    }
                }
                cnt2++;
            }

            //Loop para rampas
            cnt2 = 0;
            while (cnt2 < objCnt) {
                Nothofagus::Bellota& bellota = canvas.bellota(listaRampas[cnt2]);
                bellota.transform().location().x += -speedx * dt;
                bellota.transform().location().y += speedy * dt;
                float x = bellota.transform().location().x;
                float y = bellota.transform().location().y;
                if (y > height) {
                    bellota.transform().location().y -= height;
                    bellota.transform().location().x += rand() % 30 - 15;
                }
                if (x > width) {
                    bellota.transform().location().x -= width;
                }
                if (x < 0) {
                    bellota.transform().location().x += width;
                }
                //Manejo de colision
                if (leftX > x - spriteSize / 2 && leftX < x + spriteSize / 2 && downY > y - spriteSize / 2 && downY < y + spriteSize / 2 ||
                    leftX > x - spriteSize / 2 && leftX < x + spriteSize / 2 && upY > y - spriteSize / 2 && upY < y + spriteSize / 2 ||
                    rightX > x - spriteSize / 2 && rightX < x + spriteSize / 2 && downY > y - spriteSize / 2 && downY < y + spriteSize / 2 ||
                    rightX > x - spriteSize / 2 && rightX < x + spriteSize / 2 && upY > y - spriteSize / 2 && upY < y + spriteSize / 2) {
                    if (cooldown <= 0) {
                        soundPlayer2.play();
                        cooldown = maxCd;
                        style += 20;
                        speedy += 0.05;

                    }
                }
                cnt2++;
            }
        }

        speed = hypot(speedx, speedy);
        time_t tf;
        time(&tf);
        tf = tf - t0;
        if (!caught) {
            finalTime = tf;
        }
        string speedText = to_string((int)(speed*100));
        string timeText = to_string(finalTime);
        string styleText = to_string(style);
        dist = hypot(bellota4.transform().location().x - x0, bellota4.transform().location().y - y0);
        string distText = to_string((int)dist);


        if (!caught) {
            if (!monster && tf > monsterTime) {
                monster = true;
                bellota3.visible() = true;
            }

            if (monster) {
                float x = bellota3.transform().location().x;
                float y = bellota3.transform().location().y;
                //Manejo de colision
                if (leftX > x - spriteSize / 2 && leftX < x + spriteSize / 2 && downY > y - spriteSize / 2 && downY < y + spriteSize / 2 ||
                    leftX > x - spriteSize / 2 && leftX < x + spriteSize / 2 && upY > y - spriteSize / 2 && upY < y + spriteSize / 2 ||
                    rightX > x - spriteSize / 2 && rightX < x + spriteSize / 2 && downY > y - spriteSize / 2 && downY < y + spriteSize / 2 ||
                    rightX > x - spriteSize / 2 && rightX < x + spriteSize / 2 && upY > y - spriteSize / 2 && upY < y + spriteSize / 2) {
                    caught = true;
                    speedx = 0;
                    speedy = 0;
                    soundPlayer3.play();
                    while (soundPlayer3.isPlaying());
                }
                bellota3.transform().location().x += ((x0 - bellota3.transform().location().x) / abs(x0 - bellota3.transform().location().x)) * dt * monsterSpeed - speedx * dt;
                bellota3.transform().location().y += ((y0 - bellota3.transform().location().y) / abs(y0 - bellota3.transform().location().y)) * dt * monsterSpeed + speedy * dt;
            }
            //Manejo de la meta
            bellota4.transform().location().x += -speedx * dt;
            bellota4.transform().location().y += speedy * dt;
            float x = bellota4.transform().location().x;
            float y = bellota4.transform().location().y;
            if (leftX > x - metaX / 2 && leftX < x + metaX / 2 && downY > y - metaY / 2 && downY < y + metaY / 2 ||
                leftX > x - metaX / 2 && leftX < x + metaX / 2 && upY > y - metaY / 2 && upY < y + metaY / 2 ||
                rightX > x - metaX / 2 && rightX < x + metaX / 2 && downY > y - metaY / 2 && downY < y + metaY / 2 ||
                rightX > x - metaX / 2 && rightX < x + metaX / 2 && upY > y - metaY / 2 && upY < y + metaY / 2) {
                caught = true;
                soundPlayer4.play();
                while (soundPlayer4.isPlaying());
                
            }
            if (y > height) {
                bellota4.transform().location().y -= height + 500;
                bellota4.transform().location().x += rand() % 30 - 15;
            }
        }
        

        ImGui::Begin("Stats");
        ImGui::Text("Distance:");
        ImGui::Text(distText.data());
        ImGui::Text("Speed:");
        ImGui::Text(speedText.data());
        ImGui::Text("Time:");
        ImGui::Text(timeText.data());
        ImGui::Text("Style:");
        ImGui::Text(styleText.data());
        ImGui::End();

        if (speedy < minSpeed && !caught)
            speedy = minSpeed;

        if (!caught) {
            if (leftKeyPressed)
                speedx += -acceleration * dt;
            if (rightKeyPressed)
                speedx += acceleration * dt;
            if (downKeyPressed)
                speedy += acceleration * dt;
            if (upKeyPressed && (speedy > minSpeed))
                speedy += -acceleration * dt;

        }
        };




    Nothofagus::Controller controller;

    controller.registerAction({ Nothofagus::Key::ESCAPE, Nothofagus::DiscreteTrigger::Press }, [&]() { canvas.close(); });
    controller.registerAction({ Nothofagus::Key::A, Nothofagus::DiscreteTrigger::Press }, [&]()
        {
            leftKeyPressed = true;
        });
    controller.registerAction({ Nothofagus::Key::A, Nothofagus::DiscreteTrigger::Release }, [&]()
        {
            leftKeyPressed = false;
        });
    controller.registerAction({ Nothofagus::Key::D, Nothofagus::DiscreteTrigger::Press }, [&]()
        {
            rightKeyPressed = true;
        });
    controller.registerAction({ Nothofagus::Key::D, Nothofagus::DiscreteTrigger::Release }, [&]()
        {
            rightKeyPressed = false;
        });
    controller.registerAction({ Nothofagus::Key::S, Nothofagus::DiscreteTrigger::Press }, [&]()
        {
            downKeyPressed = true;
        });
    controller.registerAction({ Nothofagus::Key::S, Nothofagus::DiscreteTrigger::Release }, [&]()
        {
            downKeyPressed = false;
        });
    controller.registerAction({ Nothofagus::Key::W, Nothofagus::DiscreteTrigger::Press }, [&]()
        {
            upKeyPressed = true;
        });
    controller.registerAction({ Nothofagus::Key::W, Nothofagus::DiscreteTrigger::Release }, [&]()
        {
            upKeyPressed = false;
        });
    canvas.run(update, controller);

    return 0;
}

    