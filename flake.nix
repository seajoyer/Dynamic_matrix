{
  description = "Dynamic matrix project";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        testDynamicMatrics = pkgs.stdenv.mkDerivation {
          pname = "run_tests";
          version = "0.1.0";
          name = "dynamic_matrix_tests-0.1.0";

          src = ./.;

          nativeBuildInputs = with pkgs; [ gnumake ];

          buildInputs = with pkgs; [ libcxx catch2 ];

          buildPhase = ''
            make build/run_tests -j $NIX_BUILD_CORES
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp build/run_tests $out/bin/
          '';
        };

      in {
        packages = {
          dynamic_matrix = testDynamicMatrics;
          default = testDynamicMatrics;
        };

        apps = {
          dynamic_matrix = flake-utils.lib.mkApp { drv = testDynamicMatrics; };
          default = flake-utils.lib.mkApp { drv = testDynamicMatrics; };
        };

        devShells.default = pkgs.mkShell {
          name = "dynamic_matrix-dev-shell";

          nativeBuildInputs = with pkgs; [ gnumake ccache git bear ];

          buildInputs = with pkgs; [ libcxx catch2 ];

          shellHook = ''
            export CC=gcc
            export CXX=g++
            export CXXFLAGS="''${CXXFLAGS:-}"

            export CCACHE_DIR=$HOME/.ccache
            export PATH="$HOME/.ccache/bin:$PATH"
            export CPATH=${pkgs.catch2}/include:$CPATH

            alias c=clear

            echo "======================================"
            echo "$(gcc    --version | head -n 1)"
            echo "$(make   --version | head -n 1)"
            echo ""
            echo "Build the project:  nix build"
            echo "Run the project:    nix run"
            echo "Run the tests:      nix flake check"
            echo ""
            echo "Happy coding!"
          '';
        };
      });
}
