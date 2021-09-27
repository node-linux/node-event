declare module 'event' {
    export type Event = {
        time: number,
        code: number,
        type: number,
        value: number
    };

    export function open(event: string): {
        next(): Event;
        close();
    }
}

export namespace event {}
