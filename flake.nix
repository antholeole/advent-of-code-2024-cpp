{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = {nixpkgs, ...}: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in {
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        libgcc
        bear
        gtest
        pkg-config
        valgrind
      ];

      buildInputs = with pkgs; [
        gtest
      ];

      shellHook = ''
        # make the compilation database
        make compile_commands
      '';
    };
  };
}
