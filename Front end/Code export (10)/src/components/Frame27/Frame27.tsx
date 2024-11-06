import { memo } from 'react';
import type { FC } from 'react';

import resets from '../_resets.module.css';
import { AppleIPadPro11_SilverLandscape } from './AppleIPadPro11_SilverLandscape/AppleIPadPro11_SilverLandscape.js';
import { Arrow_back } from './Arrow_back/Arrow_back.js';
import { Arrow_backIcon } from './Arrow_backIcon.js';
import classes from './Frame27.module.css';
import { LinearDeterminateProgressIndic } from './LinearDeterminateProgressIndic/LinearDeterminateProgressIndic.js';
import { LinearIndeterminateProgressInd } from './LinearIndeterminateProgressInd/LinearIndeterminateProgressInd.js';

interface Props {
  className?: string;
  hide?: {
    rectangle1?: boolean;
    group13?: boolean;
  };
}
/* @figmaId 207:302 */
export const Frame27: FC<Props> = memo(function Frame27(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <AppleIPadPro11_SilverLandscape
        className={classes.appleIPadPro11_SilverLandscape2}
        classes={{ appleIPadPro11_SilverLandscape: classes.appleIPadPro11_SilverLandscape }}
        hide={{
          rectangle1: true,
          group13: true,
        }}
      />
      <Arrow_back
        className={classes.arrow_back}
        swap={{
          icon: <Arrow_backIcon className={classes.icon} />,
        }}
      />
      <div className={classes.rectangle10}></div>
      <div className={classes.rectangle9}></div>
      <div className={classes.hKGTP}>
        <p className={classes.labelWrapper}>
          <span className={classes.label}>HKG-&gt;TP</span>
          <span className={classes.label2}>|</span>
        </p>
      </div>
      <div className={classes.rectangle8}></div>
      <div className={classes.cX334}>CX334</div>
      <div className={classes.planeDetails}>Plane details</div>
      <div className={classes.rectangle6}></div>
      <div className={classes.rule2}>Rule 2</div>
      <LinearDeterminateProgressIndic
        className={classes.linearDeterminateProgressIndic}
        classes={{ track: classes.track, track2: classes.track2, trackAndStop: classes.trackAndStop }}
      />
      <LinearIndeterminateProgressInd className={classes.linearIndeterminateProgressInd} />
      <div className={classes.rectangle7}></div>
      <div className={classes.rule3}>Rule 3</div>
      <div className={classes.rectangle5}></div>
      <div className={classes.rule1}>Rule 1</div>
    </div>
  );
});
