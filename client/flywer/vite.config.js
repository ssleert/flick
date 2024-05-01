import { defineConfig } from "vite"
import basicSsl from '@vitejs/plugin-basic-ssl'
import { resolve } from "path"

// https://vitejs.dev/config/
export default defineConfig({
  target: ["es2015", "firefox54", "safari13"],
  esbuild: {
    jsx: "automatic",
    jsxImportSource: "hywer",
  },
  plugins: [
    basicSsl()
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
