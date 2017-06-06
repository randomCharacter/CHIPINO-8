#ifndef SPEAKER_H_INCLUDED
#define SPEAKER_H_INCLUDED

/**
 * Piezo buzzer representing speaker.
 */
class Speaker {
    private:
        // Number of pin buzzer is connected to
        int pin;
        
    public:
        /**
         * Default constructor.
         * 
         * @param pin Number of pin buzzer is connected to
         */
        Speaker(int pin);

        /**
         * Sets the pin to <code>HIGH</code>
         */
        void playSound();

        /**
         * Sets the pin to <code>LOW</code>
         */
        void muteSound();
};

#endif
