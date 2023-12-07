#include <jack/jack.h>
#include <iostream>
#include "SoundTouch.h"

using namespace soundtouch;
using namespace std;

jack_client_t *client;
jack_port_t *input_port;
jack_port_t *output_port1;
jack_port_t *output_port2;
SoundTouch soundTouch;

int process(jack_nframes_t nframes, void *arg) {
    float *in, *out1, *out2;
    in = (float *) jack_port_get_buffer(input_port, nframes);
    out1 = (float *) jack_port_get_buffer(output_port1, nframes);
    out2 = (float *) jack_port_get_buffer(output_port2, nframes);

    soundTouch.putSamples(in, nframes);
    soundTouch.receiveSamples(out1, nframes);

    memcpy(out2, in, sizeof(float) * nframes);

    return 0;
}

int main() {
    const char *client_name = "AudioProcessor";
    const char *server_name = nullptr;
    jack_options_t options = JackNullOption;
    jack_status_t status;

    client = jack_client_open(client_name, options, &status, server_name);
    if (client == nullptr) {
        cerr << "jack_client_open() failed, status = " << status << endl;
        if (status & JackServerFailed) {
            cerr << "Unable to connect to JACK server" << endl;
        }
        return 1;
    }

    jack_set_process_callback(client, process, 0);

    input_port = jack_port_register(client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    output_port1 = jack_port_register(client, "output1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    output_port2 = jack_port_register(client, "output2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    if (jack_activate(client)) {
        cerr << "Cannot activate client" << endl;
        return 1;
    }

    soundTouch.setSampleRate(jack_get_sample_rate(client));
    soundTouch.setChannels(1);
    soundTouch.setPitch(0.04);

    while (true) {
        sleep(1);
    }

    jack_client_close(client);
    return 0;
}
