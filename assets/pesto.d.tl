-- Pesto 0.1.0 alpha type definitions

-- Enums

-- Audio
global record SourceType
    Static: string
    Stream: string
end

-- Data
global record HashFunction
    Crc32: string
    Md5: string
    Sha1: string
    Sha256: string
end

-- Graphics
global record BlendMode
    Alpha: string
    Additive: string
    Multiplied: string
    AddColors: string
    SubtractColors: string
    AlphaPremultiply: string
end

global record DrawMode
    Fill: string
    Line: string
end

global record FilterMode
    Point: string
    Bilinear: string
    Trilinear: string
    Anisotropic4x: string
    Anisotropic8x: string
    Anisotropic16x: string
end

global record PixelFormat
    Grayscale: string
    GrayscaleAlpha: string
    RGB565: string
    RGB888: string
    RGBA5551: string
    RGBA4444: string
    RGBA8888: string
    R32: string
    R32G32B32: string
    R32G32B32A32: string
    DXT1: string
    DXT1Alpha: string
    DXT3: string
    DXT5: string
    ETC1: string
    ETC2: string
    ETC2Alpha: string
    PVRT: string
    PVRTAlpha: string
    ASTC4x4: string
    ASTC8x8: string
end

global record WrapMode
    Repeat: string
    Clamp: string
    MirrorRepeat: string
    MirrorClamp: string
end

-- Math
global record NoiseType
    OpenSimplex: string
    Perlin: string
end

-- Keyboard
global record GamepadButton
    Unknown: string
    LeftFaceUp: string
    LeftFaceRight: string
    LeftFaceDown: string
    LeftFaceLeft: string
    RightFaceUp: string
    RightFaceRight: string
    RightFaceDown: string
    RightFaceLeft: string
    LeftTrigger1: string
    LeftTrigger2: string
    RightTrigger1: string
    RightTrigger2: string
    MiddleLeft: string
    Middle: string
    MiddleRight: string
    LeftThumb: string
    RightThumb: string
end

global record GamepadAxis
    LeftX: string
    LeftY: string
    RightX: string
    RightY: string
    LeftTrigger: string
    RightTrigger: string
end

global record Key
    Null: string
    Apostrophe: string
    Comma: string
    Minus: string
    Period: string
    Slash: string
    Zero: string
    One: string
    Two: string
    Three: string
    Four: string
    Five: string
    Six: string
    Seven: string
    Eight: string
    Nine: string
    Semicolon: string
    Equal: string
    A: string
    B: string
    C: string
    D: string
    E: string
    F: string
    G: string
    H: string
    I: string
    J: string
    K: string
    L: string
    M: string
    N: string
    O: string
    P: string
    Q: string
    R: string
    S: string
    T: string
    U: string
    V: string
    W: string
    X: string
    Y: string
    Z: string
    LeftBracket: string
    Backslash: string
    RightBracket: string
    Grave: string
    Space: string
    Escape: string
    Enter: string
    Tab: string
    Backspace: string
    Insert: string
    Delete: string
    Right: string
    Left: string
    Down: string
    Up: string
    PageUp: string
    PageDown: string
    Home: string
    End: string
    CapsLock: string
    ScrollLock: string
    NumLock: string
    PrintScreen: string
    Pause: string
    F1: string
    F2: string
    F3: string
    F4: string
    F5: string
    F6: string
    F7: string
    F8: string
    F9: string
    F10: string
    F11: string
    F12: string
    LeftShift: string
    LeftControl: string
    LeftAlt: string
    LeftSuper: string
    RightShift: string
    RightControl: string
    RightAlt: string
    RightSuper: string
    KbMenu: string
    Kp0: string
    Kp1: string
    Kp2: string
    Kp3: string
    Kp4: string
    Kp5: string
    Kp6: string
    Kp7: string
    Kp8: string
    Kp9: string
    KpDecimal: string
    KpDivide: string
    KpMultiply: string
    KpSubtract: string
    KpAdd: string
    KpEnter: string
    KpEqual: string
    Back: string
    Menu: string
    VolumeUp: string
    VolumeDown: string
end

-- Mouse
global record Button
    Left: string
    Right: string
    Middle: string
    Side: string
    Extra: string
    Forward: string
    Back: string
end

global record Cursor
    Default: string
    Arrow: string
    Ibeam: string
    Crosshair: string
    PointingHand: string
    ResizeEW: string
    ResizeNS: string
    ResizeNWSE: string
    ResizeNESW: string
    ResizeAll: string
    NotAllowed: string
end

-- System
global record OS
    Windows: string
    Linux: string
    MacOS: string
end

-- Timer
global record TweenType
    Linear: string
    LinearIn: string
    LinearOut: string
    LinearInOut: string
    SineIn: string
    SineOut: string
    SineInOut: string
    CircularIn: string
    CircularOut: string
    CircularInOut: string
    CubicIn: string
    CubicOut: string
    CubicInOut: string
    QuadraticIn: string
    QuadraticOut: string
    QuadraticInOut: string
    ExponentialIn: string
    ExponentialOut: string
    ExponentialInOut: string
    BackIn: string
    BackOut: string
    BackInOut: string
    BounceIn: string
    BounceOut: string
    BounceInOut: string
    ElasticIn: string
    ElasticOut: string
    ElasticInOut: string
end

-- Window
global record Flag
    VSync: string
    Fullscreen: string
    Resizable: string
    Undecorated: string
    Hidden: string
    Minimized: string
    Maximized: string
    Unfocused: string
    Topmost: string
    AlwaysRun: string
    Transparent: string
    HighDPI: string
    MousePassthrough: string
    MSAA4X: string
    Interlaced: string
end

-- Types

-- Audio
global record Source
    type: function(self: Source): string
    unload: function(self: Source)
    getSourceType: function(self: Source): string
    getChannelCount: function(self: Source): number
    getSampleRate: function(self: Source): number
    getSampleSize: function(self: Source): number
    getSampleCount: function(self: Source): number
    getPitch: function(self: Source): number
    getVolume: function(self: Source): number
    getPan: function(self: Source): number
    getDuration: function(self: Source): number
    getPosition: function(self: Source): number, number
    getMaxDistance: function(self: Source): number
    isPlaying: function(self: Source): boolean
    isLooping: function(self: Source): boolean
    play: function(self: Source)
    playSpatial: function(self: Source, listenerX: number, listenerY: number)
    pause: function(self: Source)
    resume: function(self: Source)
    stop: function(self: Source)
    seek: function(self: Source, time: number)
    setPitch: function(self: Source, pitch: number)
    setVolume: function(self: Source, volume: number)
    setPan: function(self: Source, pan: number)
    setLooping: function(self: Source, looping: boolean)
    setPosition: function(self: Source, x: number, y: number)
    setMaxDistance: function(self: Source, maxDistance: number)
end

-- Data
global record Data
    type: function(self: Data): string
    unload: function(self: Data)
    clone: function(self: Data): Data
    getSize: function(self: Data): number
    getData: function(self: Data): table
    append: function(self: Data, data: Data)
end

-- Gamepad
global record Gamepad
    type: function(self: Gamepad): string
    getId: function(self: Gamepad): number
    getName: function(self: Gamepad): string
    isDown: function(self: Gamepad, button: string): boolean
    isPressed: function(self: Gamepad, button: string): boolean
    isReleased: function(self: Gamepad, button: string): boolean
    getAxisCount: function(self: Gamepad): number
    getAxisMovement: function(self: Gamepad, axis: string): number
end

-- Graphics
global record Camera
    type: function(self: Camera): string
    setTarget: function(self: Camera, x: number, y: number)
    setOffset: function(self: Camera, x: number, y: number)
    setRotation: function(self: Camera, rotation: number)
    setZoom: function(self: Camera, zoom: number)
    getTarget: function(self: Camera): number, number
    getOffset: function(self: Camera): number, number
    getRotation: function(self: Camera): number
    getZoom: function(self: Camera): number
    getScreenToWorld: function(self: Camera, x: number, y: number): number, number
    getWorldToScreen: function(self: Camera, x: number, y: number): number, number
end

global record Canvas
    type: function(self: Canvas): string
    unload: function(self: Canvas)
    getDimensions: function(self: Canvas): number, number
    getFilter: function(self: Canvas): string
    getFormat: function(self: Canvas): string
    getHeight: function(self: Canvas): number
    getMipmapCount: function(self: Canvas): number
    getWidth: function(self: Canvas): number
    getWrap: function(self: Canvas): string
    setFilter: function(self: Canvas, filter: string)
    setWrap: function(self: Canvas, wrap: string)
end

global record Font
    type: function(self: Font): string
    unload: function(self: Font)
    getFilter: function(self: Font): string
    getHeight: function(self: Font): number
    getWidth: function(self: Font, text: string): number
    setFilter: function(self: Font, filter: string)
end

global record Image
    type: function(self: Image): string
    unload: function(self: Image)
    replacePixels: function(self: Image, data: Data)
    getDimensions: function(self: Image): number, number
    getFilter: function(self: Image): string
    getFormat: function(self: Image): string
    getHeight: function(self: Image): number
    getMipmapCount: function(self: Image): number
    getWidth: function(self: Image): number
    getWrap: function(self: Image): string
    setFilter: function(self: Image, filter: string)
    setWrap: function(self: Image, wrap: string)
end

global record Shader
    type: function(self: Shader): string
    unload: function(self: Shader)
    hasUniform: function(self: Shader, name: string): boolean
    send: function(self: Shader, name: string, value: number)
    send: function(self: Shader, name: string, value: boolean)
    send: function(self: Shader, name: string, value: table)
    send: function(self: Shader, name: string, value: Image)
end

-- Math
global record Vector
    x: number
    y: number

    type: function(self: Vector): string
    add: function(self: Vector, other: Vector): Vector
    add: function(self: Vector, value: number): Vector
    sub: function(self: Vector, other: Vector): Vector
    sub: function(self: Vector, value: number): Vector
    scale: function(self: Vector, scale: number): Vector
    mul: function(self: Vector, other: Vector): Vector
    div: function(self: Vector, other: Vector): Vector
    normalize: function(self: Vector): Vector
    rotate: function(self: Vector, angle: number): Vector
    length: function(self: Vector): number
    distance: function(self: Vector, other: Vector): number
    dot: function(self: Vector, other: Vector): number
    angle: function(self: Vector, other: Vector): number
end

-- Functions
global record pesto
    getVersion: function(): number, number, number, string

    record ai
        setWorldSize: function(width: number, height: number)
        setDiagonalMovement: function(diagonalMovement: boolean)
        findPath: function(startX: number, startY: number, endX: number, endY: number): {Vector}
        addCollision: function(x: number, y: number)
        removeCollision: function(x: number, y: number)
        clearCollisions: function()
        getDiagonalMovement: function(): boolean
    end

    record audio
        getActiveSourceCount: function(): number
        getVolume: function(): number
        newSource: function(filename: string, type: string): Source
        setVolume: function(volume: number)
        stopAllSources: function()
    end

    record data
        compress: function(data: Data): Data
        decode: function(data: string): Data
        decompress: function(data: Data): Data
        encode: function(data: Data): string
        hash: function(data: string, hashFunction: string): string
        hash: function(data: Data, HashFunction: string): string
        newData: function(data: table): Data
        printTable: function(table: table)
    end

    record event
        run: function(filename: string)
        reload: function()
        quit: function()
        add: function(name: string, func: function)
        remove: function(name: string)
        call: function(name: string, ...: any)
        clear: function()
    end

    record filesystem
        loadFileData: function(filename: string): Data
        saveFileData: function(filename: string, data: Data)
        loadFileText: function(filename: string): string
        saveFileText: function(filename: string, text: string)
        fileExists: function(filename: string): boolean
        directoryExists: function(path: string): boolean
        isFileExtension: function(filename: string, ext: string): boolean
        getFileSize: function(filename: string): number
        getFileExtension: function(filename: string): string
        getFileName: function(filename: string): string
        getFileNameWithoutExtension: function(filename: string): string
        getDirectoryPath: function(filename: string): string
        getPrevDirectoryPath: function(filename: string): string
        getWorkingDirectory: function(): string
        getAppDirectory: function(): string
        changeDirectory: function(path: string): boolean
        isPathFile: function(path: string): boolean
        getDirectoryFiles: function(path: string): {string}
        getFileModTime: function(filename: string): number
        createDirectory: function(path: string): boolean
        removeFile: function(filename: string): boolean
        removeDirectory: function(path: string): boolean
        copyFile: function(src: string, dst: string)
        moveFile: function(src: string, dst: string)
    end

    record gamepad
        getGamepads: function(): {Gamepad}
        getGamepadCount: function(): number
    end

    record graphics
        circle: function(mode: string, x: number, y: number, radius: number)
        clear: function(r: number, g: number, b: number)
        clear: function(r: number, g: number, b: number, a: number)
        draw: function(image: Image, x: number, y: number)
        draw: function(image: Image, x: number, y: number, rotation: number, scale: number)
        draw: function(canvas: Canvas, x: number, y: number)
        ellipse: function(mode: string, x: number, y: number, radiusX: number, radiusY: number)
        line: function(x1: number, y1: number, x2: number, y2: number)
        point: function(x: number, y: number)
        polygon: function(mode: string, x: number, y: number, sides: number, radius: number)
        polygon: function(mode: string, x: number, y: number, sides: number, radius: number, rotation: number)
        print: function(text: string, x: number, y: number)
        print: function(text: string, x: number, y: number, rotation: number, scale: number)
        rectangle: function(mode: string, x: number, y: number, width: number, height: number)
        rectangle: function(mode: string, x: number, y: number, width: number, height: number, rotation: number)
        triangle: function(mode: string, x1: number, y1: number, x2: number, y2: number, x3: number, y3: number)
        captureScreenshot: function(filename: string)
        newCamera: function(): Camera
        newCanvas: function(width: number, height: number): Canvas
        newFont: function(filename: string, size: number): Font
        newImage: function(filename: string): Image
        newShader: function(vertex: string, fragment: string): Shader
        getBackgroundColor: function(): number, number, number, number
        getColor: function(): number, number, number, number
        getDefaultFilter: function(): string
        getFont: function(): Font
        setBackgroundColor: function(r: number, g: number, b: number)
        setBackgroundColor: function(r: number, g: number, b: number, a: number)
        setBlendMode: function(mode: string)
        setBlendMode: function()
        setCamera: function(camera: Camera)
        setCamera: function()
        setCanvas: function(canvas: Canvas)
        setCanvas: function()
        setColor: function(r: number, g: number, b: number)
        setColor: function(r: number, g: number, b: number, a: number)
        setDefaultFilter: function(filter: string)
        setFont: function(font: Font)
        setShader: function(shader: Shader)
        setShader: function()
    end

    record gui

    end

    record keyboard
        isDown: function(key: string): boolean
        isPressed: function(key: string): boolean
        isReleased: function(key: string): boolean
    end

    record math
        clamp: function(value: number, min: number, max: number): number
        getNoiseFrequency: function(): number
        getNoiseType: function(): string
        getNoiseSeed: function(): number
        getRandomSeed: function(): number
        lerp: function(from: number, to: number, amount: number): number
        newVector2: function(x: number, y: number): Vector
        noise: function(x: number, y: number): number
        noise: function(x: number, y: number, z: number): number
        normalize: function(value: number, from: number, to: number): number
        random: function(): number
        random: function(max: number): number
        random: function(min: number, max: number): number
        remap: function(value: number, inputStart: number, inputEnd: number, outputStart: number, outputEnd: number): number
        setNoiseFrequency: function(frequency: number)
        setNoiseType: function(type: string)
        setNoiseSeed: function(seed: number)
        setRandomSeed: function(seed: number)
        wrap: function(value: number, min: number, max: number): number
    end

    record mouse
        getPosition: function(): number, number
        getDelta: function(): number, number
        getWheelMove: function(): number
        getX: function(): number
        getY: function(): number
        isDown: function(button: string): boolean
        isPressed: function(button: string): boolean
        isReleased: function(button: string): boolean
        isGrabbed: function(): boolean
        isVisible: function(): boolean
        setCursor: function(cursor: string)
        setGrabbed: function(grabbed: boolean)
        setPosition: function(x: number, y: number)
        setVisible: function(visible: boolean)
        setX: function(x: number)
        setY: function(y: number)
    end

    record networking
        connect: function(address: string)
        disconnect: function()
        sendReliable: function(data: Data)
        sendUnreliable: function(data: Data)
    end

    record physics

    end

    record system
        getClipboardText: function(): string
        getOS: function(): string
        openURL: function(url: string)
        setClipboardText: function(text: string)
    end

    record timer
        getAverageDelta: function(): number
        getDelta: function(): number
        getFPS: function(): number
        getTime: function(): number
        sleep: function(seconds: number)

        after: function(seconds: number, func: function): number
        every: function(seconds: number, func: function): number
        every: function(seconds: number, func: function, times: number): number
        during: function(seconds: number, func: function): number
        during: function(seconds: number, func: function, after: function): number
        cancel: function(id: number)
        clear: function()
        tween: function(timer: number, from: number, to: number, seconds: number): number
        tween: function(timer: number, from: number, to: number, seconds: number, type: string): number
    end

    record window
        enableLetterbox: function(width: number, height: number)
        disableLetterbox: function()
        getDPIScale: function(): number, number
        getDesktopDimensions: function(display: number): number, number
        getDimensions: function(): number, number
        getDisplayCount: function(): number
        getDisplayName: function(display: number): string
        getFullscreen: function(): boolean
        getHeight: function(): number
        getIcon: function(): Image
        getLetterbox: function(): boolean
        getMode: function(): table
        getPosition: function(): number, number
        getTitle: function(): string
        getVSync: function(): boolean
        getWidth: function(): number
        hasFocus: function(): boolean
        isMaximized: function(): boolean
        isMinimized: function(): boolean
        isOpen: function(): boolean
        isResized: function(): boolean
        isVisible: function(): boolean
        maximize: function()
        minimize: function()
        restore: function()
        setFullscreen: function(fullscreen: boolean)
        setIcon: function(image: Image)
        setMinSize: function(width: number, height: number)
        setMode: function(flags: table)
        setPosition: function(x: number, y: number)
        setTitle: function(title: string)
        setVSync: function(vsync: boolean)
        unsetMode: function(flags: table)
    end
end
