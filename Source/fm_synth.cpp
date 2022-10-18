#include <stdio.h>
#include <stdlib.h> //for exit()
#include <math.h>   //for sin()
#include "fm_synth.h"

/* constructor */
FmSynth::FmSynth(void){
    ;
}

/* destructor */
FmSynth::~FmSynth(void)
{
    ;
}

void FmSynth::init_synth(int in_samp_rate)
{
    Tone *pt = &this->tone[0]; /* Tone structure */
    this->num_keys = 0; //number of keys voiced
    for (int i=0; i<KEYS_VOICED; i++) {
        pt[i].key = ' '; //space
    }
    this->samp_rate = in_samp_rate;
    this->ampl = FS_AMPL;
    this->attack_factor = ATTACK_FACTOR;
    this->decay_factor = 0.9999739587;//DECAY_FACTOR;
    this->drop_level = DROP_LEVEL;
}

void FmSynth::set_ampl(double in_ampl)
{
    this->ampl = in_ampl;
}

void FmSynth::set_fm1(double fm1)
{
    this->fm1 = fm1;
}

void FmSynth::set_fm2(double fm2)
{
    this->fm2 = fm2;
}

void FmSynth::set_beta1(double beta1)
{
    this->beta1 = beta1;
}

void FmSynth::set_beta2(double beta2)
{
    this->beta2 = beta2;
}

static double decay[] = {
    0.9896373989,	//0.002	 seconds
    0.9995834201,	//0.050	 seconds
    0.9997916884,	//0.100	 seconds
    0.9998958388,	//0.200	 seconds
    0.9999479180,	//0.400	 seconds
    0.9999739587,	//0.800	 seconds
    0.9999826390,	//1.200	 seconds
    0.9999851192,	//1.400	 seconds
    0.9999884260,	//1.800	 seconds
    0.9999895834	//2.000	 seconds
};
static double attack[] = {
    0.9896373989,    //0.002     seconds
    0.9995834201,    //0.050     seconds
    0.9997916884,    //0.100     seconds
    0.9998958388,    //0.200     seconds
    0.9999479180,    //0.400     seconds
    0.9999739587,    //0.800     seconds
    0.9999826390,    //1.200     seconds
    0.9999851192,    //1.400     seconds
    0.9999884260,    //1.800     seconds
    0.9999895834    //2.000     seconds
//    0.9999895834,
//    0.9999884260,
//    0.9999851192,
//    0.9999826390,
//    0.9999739587,
//    0.9999479180,
//    0.9998958388,
//    0.9997916884,
//    0.9995834201,
//    0.9896373989,
};

void FmSynth::set_attack(double range)
{
    int i = 10 * range;
    if (i < 0) i = 0;
    if (i > 9) i = 9;
    this->attack_factor = attack[i];
}
void FmSynth::set_decay(double range)
{
    int i = 10*range;
    if (i<0) i=0;
    if (i>9) i=9;
    this->decay_factor = decay[i];
}

void FmSynth::execute_cmd(int in_cmd)
{
    switch (in_cmd) {
        case (CMD_ADD_KEY):
            add_key(this->new_freq);
            break;
        case (CMD_RM_KEY):
            rm_key();
            break;
    }
}

float *FmSynth::synth_block(int framesPerBuffer)
{
    /* check for actions from user interface thread */
    //Your Code Here
    //Begin
    int local_cmd = this->cmd; /* atomic read to local copy */
    if (local_cmd > 0) {
        execute_cmd(local_cmd);
        /* and reset cmd */
        this->cmd = 0; /* atomic write to reset */
    }
    //End

    /* synthesize tones */
    //Begin
    Tone *pt = &this->tone[0]; /* Tone structure */
    int k = 0;
    for (int i=0; i<framesPerBuffer; i++) {
        /* for each sample in frame */
        double v = 0.0;
        /* for each key voiced */
        for (int n=0; n<this->num_keys; n++) {
            /* synthesize tone */
            if (pt[n].phase_inc > 0) {
                //determine the waveform type of the carrier
                double car = 0.0;
                if (this->car == 0)
                {
                    car = pt[n].phase;
                }
                if (this->car == 1)
                {
                    if(pt[n].phase < PI)
                    {
                        car = -1 * PI / 2;
                    }
                    else
                    {
                        car = PI / 2;
                    }
                }
                if (this->car == 2)
                {
                    car = pt[n].phase / 2 * PI;
                    car = 2*(car-0.5);
                }
                
                //determine the type of modulators 1 & 2
                //modify phase_inc accordingly
                double mod1 = 0.0;
                double mod2 = 0.0;
            
                //modulator 1
                if (this->mod1 == 0)
                {
                    //sine
                    mod1 = pt[n].phase_mod1;
                }
                if (this->mod1 == 1)
                {
                    //square
                    if (pt[n].phase_mod1 < PI)
                    {
                        mod1 = -1 * PI / 2;
                    }
                    else
                    {
                        mod1 = PI / 2;
                    }
                }
                if (this->mod1 == 2)
                {
                    //saw or tri
                    mod1 = pt[n].phase_mod1 / 2 * PI;
                    mod1 = 2*(mod1*0.5);
                }
                
                //modulator 2
                if (this->mod2 == 0)
                {
                    //sine
                    mod2 = pt[n].phase_mod2;
                }
                if (this->mod2 == 1)
                {
                    //square
                    if (pt[n].phase_mod2 < PI)
                    {
                        mod2 = -1 * PI / 2;
                    }
                    else
                    {
                        mod2 = PI / 2;
                    }
                }
                if (this->mod2 == 2)
                {
                    //saw or tri
                    mod2 = pt[n].phase_mod2 / 2 * PI;
                    mod2 = 2*(mod2-0.5);
                }
                
                /* synthesize tone only if phase_inc > 0 */
                /* sum tones together to make output */
                v += this->ampl * 
                    (1 - pt[n].attack_amp) * pt[n].decay_amp *
                    sin(car +
                    this->beta1 * sin(mod1 +
                    this->beta2 * sin(mod2)));
                
                /* update phase and attack, decay amplitudes */
                pt[n].phase += pt[n].phase_inc;
                //update modulators
                pt[n].phase_mod1 += pt[n].phase_inc1;
                pt[n].phase_mod2 += pt[n].phase_inc2;
                
                //re-range carrier phase if it's a sqaure wave
                if (this->car == 1)
                {
                    if (pt[n].phase > 2 * PI)
                    {
                        pt[n].phase -= 2 * PI;
                    }
                }
                
                if (this->car == 2)
                {
                    if (pt[n].phase > 2 * PI)
                    {
                        pt[n].phase -= 2 * PI;
                    }
                }
                
                //re-range modulators if they're square waves
                if (this->mod1 == 1)
                {
                    if (pt[n].phase_mod1 > 2 *PI)
                    {
                        pt[n].phase_mod1 -= 2 * PI;
                    }
                }
                
                if (this->mod2 == 1)
                {
                    if (pt[n].phase_mod2 > 2 * PI)
                    {
                        pt[n].phase_mod2 -= 2 * PI;
                    }
                }
                
                pt[n].attack_amp *= pt[n].attack_factor;
                pt[n].decay_amp *= pt[n].decay_factor;
                /* if tone is below threshold amplitude, stop tone playout */
                if (pt[n].decay_amp < this->drop_level) {
                    rm_key();
                }
            }
        }
        this->output[k++] = v;
    }
    //End
    return( &output[0] );
}

void FmSynth::add_key(double new_freq)
{
    this->new_freq = new_freq;
    /* increment number of keys */
    if (++(this->num_keys) > KEYS_VOICED) {
        /* list is full, so remove oldest key by shifting key list */
        shift_keys();
        this->num_keys = KEYS_VOICED;
    }
    /* enter new key info */
    init_key(new_freq);
}

void FmSynth::rm_key(void)
{
    /* remove oldest key by shifting key list down */
    shift_keys();
    /* decreast number of keys voicing */
    /* number of keys can never be less than zero */
    if (--(this->num_keys) < 0) {
        this->num_keys = 0;
    }
}

void FmSynth::shift_keys(void)
{
    Tone *pt = &this->tone[0];
    /* shift tone array values down by one place */
    for (int i=0; i<KEYS_VOICED-1; i++) {
        pt[i].key = pt[i+1].key;
        pt[i].f0 = pt[i+1].f0;
        pt[i].phase_inc = pt[i+1].phase_inc;
        pt[i].phase = pt[i+1].phase;
        
        //shift modulator keys
        pt[i].phase_mod1 = pt[i+1].phase_mod1;
        pt[i].phase_mod2 = pt[i+1].phase_mod2;
        
        pt[i].attack_factor = pt[i+1].attack_factor;
        pt[i].decay_factor = pt[i+1].decay_factor;
        pt[i].attack_amp = pt[i+1].attack_amp;
        pt[i].decay_amp = pt[i+1].decay_amp;
    }
    /* top array entry is initialized to inactive */
    int n = KEYS_VOICED-1;
    pt[n].key = ' ';
    pt[n].phase_inc = -1;
}

void FmSynth::init_key(double freq)
{
    /* add new key information at num_keys-1 index in Tone array */
    int n;
    Tone *pt = &this->tone[0];
    n = this->num_keys-1; /* from number to index */
    if (n < 0) {
        /* since init_key() is called after ++num_keys, this should not occur */
        fprintf(stderr, "ERROR in init_key(): num_keys %d\n", this->num_keys);
        exit(-1);
    }
    pt[n].attack_factor = attack_factor;
    pt[n].decay_factor = decay_factor;
    pt[n].attack_amp = 1.0;
    pt[n].decay_amp = 1.0;
    pt[n].phase_inc = 2*PI*freq/this->samp_rate;
    pt[n].phase = 0.0;
    
    //modulator phase init
    pt[n].phase_mod1 = 0.0;
    pt[n].phase_mod2 = 0.0;
    pt[n].phase_inc1 = 2*PI*this->fm1/this->samp_rate;
    pt[n].phase_inc2 = 2*PI*this->fm2/this->samp_rate;
}

void FmSynth::set_car_sin(void)
{
    this->car = 0;
}

void FmSynth::set_car_sq(void)
{
    this->car = 1;
}

void FmSynth::set_car_saw(void)
{
    this->car = 2;
}

void FmSynth::set_mod1_sin(void)
{
    this->mod1 = 0;
}

void FmSynth::set_mod1_sq(void)
{
    this->mod1 = 1;
}

void FmSynth::set_mod1_saw(void)
{
    this->mod1 = 2;
}

void FmSynth::set_mod2_sin(void)
{
    this->mod2 = 0;
}

void FmSynth::set_mod2_sq(void)
{
    this->mod2 = 1;
}

void FmSynth::set_mod2_saw(void)
{
    this->mod2 = 2;
}
