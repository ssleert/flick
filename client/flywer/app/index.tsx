import App from "@/ui/App";
import "#shared/css/root.less";

const app = document.getElementById("app")!;
app.classList.add("LightThemeColors");
app.classList.add("AppFonts");

app.append(
  ...<>
    <App />
  </>
);
