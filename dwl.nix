{stdenv, fetchFromGitHub, fetchgit, meson, ninja, scdoc, pkg-config, libuuid, wlroots_0_16, wayland, libinput, libGL, pixman, libxkbcommon, wayland-protocols, libxcb, libX11, xwayland, polkit, seatd}:

stdenv.mkDerivation {
    pname = "dwl";
    version = "0.4";
    src = ./.;
    nativeBuildInputs = [
        pkg-config
        wayland
    ];
    buildInputs = [
        wayland
        wlroots_0_16
        wayland-protocols
        libinput
        libGL
        pixman
        libxkbcommon
        libxcb
        libX11
        xwayland
        seatd
    ];
    NIX_CFLAGS_COMPILE = [ "-DXWAYLAND=0" ];
    makeFlags = [
        "PREFIX=${placeholder "out"}"
    ];
}
