{ lib
, stdenv
, fetchgit
, installShellFiles
, libX11
, libinput
, libxcb
, libxkbcommon
, pixman
, pkg-config
, substituteAll
, wayland
, wayland-protocols
, wlroots_0_16
, writeText
, xcbutilwm
, xwayland
, enableXWayland ? true

}:

let
  wlroots = wlroots_0_16;
in
stdenv.mkDerivation (self: {
  pname = "dwl";
  version = "0.4.0";

 src = fetchgit {
    url = "https://github.com/Halfarne/dwl.git";
    sha256 = "sha256-ayGidlcJyb3dkZgPSb6xDNwlzRoiKx33geIsi0l99QA=";
  }; 

  nativeBuildInputs = [
    installShellFiles
    pkg-config
  ];

  buildInputs = [
    libinput
    libxcb
    libxkbcommon
    pixman
    wayland
    wayland-protocols
    wlroots
  ] ++ lib.optionals enableXWayland [
    libX11
    xcbutilwm
    xwayland
  ];

  outputs = [ "out" "man" ];

  preBuild = ''
    makeFlagsArray+=(
      XWAYLAND=${if enableXWayland then "-DXWAYLAND" else ""}
      XLIBS=${if enableXWayland then "xcb\\ xcb-icccm" else ""}
    )
  '';

  installFlags = [
    "PREFIX=$(out)"
    "MANDIR=$(man)/share/man/man1"
  ];
})
