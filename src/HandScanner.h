/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

class HandScanner {
    public:
        HandScanner(int red, int grn, int blu, int inp);
        void setup();
        void tick();
    private:
        int _step;
        int _red;
        int _grn;
        int _blu;
        int _inp;
        int _ticks_remaining;
        int _num_scans;
        void _step_wait();
        void _step_scan();
        void _step_pass();
        void _step_fail();
};

