#include <napi.h>

#include <iostream>
#include <string>
#include <thread>

#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <unistd.h>

static const char *const evval[3] = {"RELEASED", "PRESSED ", "REPEATED"};

Napi::Object open_event(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    if (info[0].IsString()) {
        auto device = info[0].ToString().Utf8Value();
        const char *dev = device.c_str();

        struct input_event ev;

        int fd = open(dev, O_RDONLY);
        if (fd == -1)
            Napi::TypeError::New(env, std::string("Cannot open ") + dev + ": " + strerror(errno)).ThrowAsJavaScriptException();
        else {
            Napi::Object controls = Napi::Object::New(env);
            controls.Set("close", Napi::Function::New(env, [&](const Napi::CallbackInfo &info) {
                int status = close(fd);

                if (status != 0)
                    Napi::TypeError::New(env, std::string("Error closing device") + dev + ": " + strerror(status)).ThrowAsJavaScriptException();
            }));
            controls.Set("next", Napi::Function::New(env, [=, &ev](const Napi::CallbackInfo &info) -> Napi::Object {
                ssize_t n = read(fd, &ev, sizeof ev);

                Napi::Object event = Napi::Object::New(env);
                event.Set("time", Napi::Number::New(env, (double)(ev.time.tv_usec / 1000)));
                event.Set("type", Napi::Number::New(env, (double)ev.type));
                event.Set("code", Napi::Number::New(env, (double)ev.code));
                event.Set("value", Napi::Number::New(env, (double)ev.value));

                return event;
            }));

            return controls;
        }
    } else
        Napi::TypeError::New(env, "Expected string").ThrowAsJavaScriptException();

    return Napi::Object::New(env);
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("open", Napi::Function::New(env, open_event));

    return exports;
}

NODE_API_MODULE(event, Init)
