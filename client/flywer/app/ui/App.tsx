import { ref } from "hywer";
import Api from "@/data/api/Api";

import css from "@/ui/App.module.less";

const App = () => {
  const counter = ref(0);

  (async () => {
    console.log(await Api.registerUser("asd@gma123il.com", "asasdasd123d", "as123dasdasd"))
  })()

  return <div class={css.App}>
    {counter}
    <button class={css.Increment} onClick={() => counter.val++}>+</button>
    <button onClick={() => counter.val--}>-</button>
  </div>
}

export default App
