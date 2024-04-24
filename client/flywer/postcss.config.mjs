import autoprefixer from "autoprefixer"
import postcss_nested from "postcss-nested"
import postcss_color_mix from "postcss-color-mix"
import postcss_preset_env from "postcss-preset-env"
import postcss_less from "postcss-less"

const config = {
  plugins: [
    autoprefixer,
    postcss_nested,
    postcss_color_mix,
    postcss_preset_env,
  ],
  syntax: postcss_less
}

export default config;
