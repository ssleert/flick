with import <nixpkgs> {};


mkShell rec {
  packages = [
    pkg-config
    fcgi
    go-migrate
    postgresql_16
    openssl
  ];
}
