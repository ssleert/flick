import { createRouterContext, Router } from "hywer/x/router";

import NavBar from "@/ui/NavBar/NavBar";
import { routes } from "@/ui/routes";

import css from "./App.module.less";

const App = () => {
  createRouterContext(routes);

  return (
    <div class={css.Page}>
      <header class={css.NavBar}>
        <NavBar />
      </header>
      <main class={css.Content}>
        <Router />
      </main>
    </div>
  )
}

export default App
