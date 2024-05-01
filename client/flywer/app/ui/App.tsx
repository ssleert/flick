import { createRouterContext, Router } from "hywer/x/router";

import NavBar from "@/ui/widgets/NavBar/NavBar";
import NotificationsList from "@/ui/widgets/Notifications/Notifications";
import { addNotification } from "@/ui/widgets/Notifications/Notifications";
import { routes } from "@/ui/routes";

import css from "./App.module.less";

const App = () => {
  createRouterContext(routes);

  return (
    <div class={css.Page}>
      <header class={css.NavBar}>
        <NavBar />
      </header>

      <header class={css.Notifications}>
        <NotificationsList />
      </header>

      <main class={css.Content}>
        <Router />
      </main>
    </div>
  );
};

export default App;
