import { createRouterContext, Router } from "hywer/x/router";

import NavBar from "@/ui/NavBar/NavBar"

import Registration from "@/ui/pages/Registration/Registration";

const App = () => {
  const routes = {
    "/404": () => <>404 page</>,
    "/": () => <>home</>,
    "/messages": () => <>messages</>,
    "/new": () => <>new</>,
    "/search": () => <>search</>,
    "/settings": () => <>settings</>,
    "/reg": () => <Registration />,
    "/login": () => <>login</>,
  };
  createRouterContext(routes);

  return <>
    <Router />
  </>
}

export default App
