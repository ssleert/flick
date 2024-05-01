import Registration from "@/ui/pages/Registration/Registration";
import Login from "@/ui/pages/Login/Login";

export const routeNames = {
  notFound: "/404",
  root: "/",
  messages: "/messages",
  new: "/new",
  search: "/search",
  settings: "/settings",
  reg: "/reg",
  login: "/login",
};

export const routes = {
  [routeNames.notFound]: () => <>404 page</>,
  [routeNames.root]: () => <>home</>,
  [routeNames.messages]: () => <>messages</>,
  [routeNames.new]: () => <>new</>,
  [routeNames.search]: () => <>search</>,
  [routeNames.settings]: () => <>settings</>,
  [routeNames.reg]: () => <Registration />,
  [routeNames.login]: () => <Login />,
};
