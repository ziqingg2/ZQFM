#ifndef _FM_SYNTH_H_
#define _FM_SYNTH_H_

#include <stdatomic.h>  /* permits write/read of "cmd" to be atomic */

/* other defines */
#define KEYS_VOICED         6   /* number of simultaneouse keys voiced */
#define SAMP_RATE           48000
#define FRAMES_PER_BUFFER   1024
#define FS_AMPL             0.25 /* -12 dB FS */
//#define ATTACK_FACTOR       0.99980 /* attack time constant of 100 ms */
//#define ATTACK_FACTOR       0.99960 /* attack time constant of 50 ms */
#define ATTACK_FACTOR       0.99800 /* attack time constant of 10 ms */
#define DECAY_FACTOR        0.99998 /* decay time constant of 1.0 sec */
#define DROP_LEVEL          0.001  /* -60 dBFS */
#define PI                  3.14159265358979323846

/* commands */
enum {
    CMD_ADD_KEY = 1,
    CMD_RM_KEY
};

/* for each tone */
typedef struct {
    int key; /* index into freq array */   
    double f0; /* frequency associated with key */
    double fm1; //frequency of mod1
    double fm2; //frequency of mod2
    double beta1; //amp of first modulator
    double beta2; //amp of second modulator
    double phase_inc; /* phase increment per sample to realize freq */
    double phase_inc1; //phase increment per sample for the first modulator
    double phase_inc2; // phase increment per sample for the second modulator
    double phase; /* save phase value for next sample */
    double phase_mod1;//modulator phase
    double phase_mod2;//second modulator's phase
    double pm_inc;//modulator phase increment
    double attack_factor;
    double decay_factor;
    double attack_amp; /* save attack amplitude for next sample */
    double decay_amp; /* save decay amplitude for next sample */
} Tone;

class FmSynth {
public:
    FmSynth(void); //constructor
    ~FmSynth(void); //destructor
    /* public member functions */
    void init_synth(int samp_rate);
    
    //set type of the carrier
    void set_car_sin(void);
    void set_car_sq(void);
    void set_car_saw(void);
    
    //set type of modulators
    void set_mod1_sin(void);
    void set_mod2_sin(void);
    void set_mod1_sq(void);
    void set_mod2_sq(void);
    void set_mod1_saw(void);
    void set_mod2_saw(void);
    
    void set_ampl(double ampl);
    void set_fm1(double fm1);
    void set_fm2(double fm2);
    void set_beta1(double beta1);
    void set_beta2(double beta2);
    
    void set_attack(double range);
    void set_decay(double range);
    void execute_cmd(int cmd);
    float *synth_block(int len);
    void add_key(double freq);
    void rm_key(void);
    void init_key(double new_freq);
    bool invalid_key(int key);
    void shift_keys(void);

    /* public class data */
    atomic_int cmd; //command for synth
    double new_freq;//freq associated with new key
    int num_keys;   //number of keys voiced
    int num_chan;   //output channels
    int samp_rate;  //sampling rate of output
    Tone tone[KEYS_VOICED];
    double ampl;
    
    int car; //type of waveform of the carrier
    
    double beta1; //amp of first modulator
    double beta2; //amp of second modulator
    double fm1; //frequency of mod1
    double fm2; //frequency of mod2
    
    int mod1;//type of modulator for modulator 1
    int mod2; //type of modulator for modulator 2
    
    
    double attack_factor;
    double decay_factor;
    double drop_level;
    float output[FRAMES_PER_BUFFER];
#if (DB_WAV_OUT)
    SNDFILE *sndfile; //for debug file
#endif
};

#endif /* _FM_SYNTH_H_ */
