with import <nixpkgs> {};


mkShell rec {
  packages = [
    fcgi
    libpqxx
  ];
}
