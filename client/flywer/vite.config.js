import { defineConfig } from "vite"
import basicSsl from '@vitejs/plugin-basic-ssl'
import { resolve } from "path"

// https://vitejs.dev/config/
export default defineConfig({
  target: "firefox54",
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
      "@/shared": resolve(__dirname, "../shared"),
      "#root": resolve(__dirname)
    }
  },
})
