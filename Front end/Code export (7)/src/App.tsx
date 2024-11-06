import { memo } from 'react';
import type { FC } from 'react';

import classes from './App.module.css';
import resets from './components/_resets.module.css';
import { AppleIPadPro11_SilverLandscape } from './components/AppleIPadPro11_SilverLandscape/AppleIPadPro11_SilverLandscape.js';

interface Props {
  className?: string;
  hide?: {
    rectangle1?: boolean;
    group13?: boolean;
  };
}
export const App: FC<Props> = memo(function App(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <AppleIPadPro11_SilverLandscape
        className={classes.b}
        classes={{ appleIPadPro11_SilverLandscape: classes.i2 }}
        hide={{
          rectangle1: true,
          group13: true,
        }}
      />
    </div>
  );
});
