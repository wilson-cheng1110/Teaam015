import { memo } from 'react';
import type { FC } from 'react';

import resets from '../_resets.module.css';
import { AppleIPadPro11_SilverLandscape } from './AppleIPadPro11_SilverLandscape/AppleIPadPro11_SilverLandscape.js';
import { Arrow_back } from './Arrow_back/Arrow_back.js';
import { Arrow_backIcon } from './Arrow_backIcon.js';
import classes from './Frame23.module.css';

interface Props {
  className?: string;
  hide?: {
    rectangle1?: boolean;
    group13?: boolean;
  };
}
/* @figmaId 205:293 */
export const Frame23: FC<Props> = memo(function Frame23(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <div className={classes.frame22}>
        <AppleIPadPro11_SilverLandscape
          className={classes.c}
          classes={{ appleIPadPro11_SilverLandscape: classes.appleIPadPro11_SilverLandscape }}
          hide={{
            rectangle1: true,
            group13: true,
          }}
        />
      </div>
      <Arrow_back
        className={classes.arrow_back}
        swap={{
          icon: <Arrow_backIcon className={classes.icon} />,
        }}
      />
      <div className={classes.rectangle4}></div>
      <div className={classes.image2}></div>
      <div className={classes.solidWasteOverload}>Solid waste overload</div>
      <div className={classes.rectangle42}></div>
      <div className={classes.image22}></div>
      <div className={classes.cleanLiquid}>Clean liquid</div>
    </div>
  );
});
