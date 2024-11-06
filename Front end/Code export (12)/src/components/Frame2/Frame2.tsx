import { memo } from 'react';
import type { FC } from 'react';

import resets from '../_resets.module.css';
import { Arrow_back } from './Arrow_back/Arrow_back.js';
import { Arrow_backIcon } from './Arrow_backIcon.js';
import classes from './Frame2.module.css';
import { IPadPro11M4_ColorSilver } from './IPadPro11M4_ColorSilver/IPadPro11M4_ColorSilver.js';

interface Props {
  className?: string;
}
/* @figmaId 207:368 */
export const Frame2: FC<Props> = memo(function Frame2(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <IPadPro11M4_ColorSilver classes={{ iPadPro11M4SilverLandscape: classes.iPadPro11M4SilverLandscape }} />
      <div className={classes.image}></div>
      <Arrow_back
        className={classes.arrow_back}
        swap={{
          icon: <Arrow_backIcon className={classes.icon} />,
        }}
      />
    </div>
  );
});
