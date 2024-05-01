import { defineConfig } from "vite"
import { resolve } from "path"

import ssl from '@vitejs/plugin-basic-ssl';

// https://vitejs.dev/config/
export default defineConfig({
  target: ["esnext"],

  cssMinify: "lightningcss",
  minify: "terser",
  esbuild: {
    jsx: "automatic",
    jsxImportSource: "hywer",
  },
  plugins: [
    ssl(),
  ],
  resolve: {
    alias: {
      "@": resolve(__dirname, "app"),
      "#public": resolve(__dirname, "public"),
      "#shared": resolve(__dirname, "shared"),
      "#root": resolve(__dirname)
    }
  },
})
