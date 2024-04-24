import { createRouterContext, Router } from "hywer/x/router";

import NavBar from "@/ui/NavBar/NavBar"

const routes = {
  "/404": () => <>404 page</>,
  "/": () => <>home</>,
  "/messages": () => <>messages</>,
  "/new": () => <>new</>,
  "/search": () => <>search</>,
  "/settings": () => <>settings</>,
  "/reg": () => <>reg</>,
  "/login": () => <>login</>,
}

const App = () => {
  createRouterContext(routes);

  return <>
    <NavBar />
    <Router />
  </>
}

export default App
