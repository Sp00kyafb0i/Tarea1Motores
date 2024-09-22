
#include <rusty_audio.h>
#include <cmath>
#include <limits>
#include <iostream>
#include <numbers>

int main()
{
    constexpr unsigned int sampleRate = 48000;
    constexpr unsigned int channels = 2;

    RustyAudio::Builder soundBuilder;
    soundBuilder.appendSinusoids({
        {200, 0.5, 600},
        {200, 0.5, 700},
        {200, 0.5, 600},
        {200, 0.5, 700},
        {200, 0.5, 600},
        {200, 0.5, 700},
        {200, 0.5, 600},
        {200, 0.5, 700},
        {200, 0.5, 600},
        {200, 0.5, 700}
    });

    RustyAudio::Buffer soundBuffer = soundBuilder.generate(sampleRate, channels);

    RustyAudio::Player soundPlayer;
    soundPlayer.init(soundBuffer);

    std::cout << "Press enter to play";
    getchar(); 
    soundPlayer.play();

    std::cout << "Press enter to pause";
    getchar();
    soundPlayer.pause();

    std::cout << "Press enter to continue";
    getchar();
    soundPlayer.play();

    std::cout << "Press enter to stop";
    getchar();
    soundPlayer.stop();

    std::cout << "Press enter to play again";
    getchar();
    soundPlayer.play();

    std::cout << "Press enter to finish";
    getchar();
    soundPlayer.stop();
}