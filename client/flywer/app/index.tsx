import App from "@/ui/App";
import "#shared/css/root.less";

const app = document.getElementById("app")!;
app.classList.add("LightThemeColors");

app.append(
  ...<>
    <App />
  </>
);
