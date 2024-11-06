import { memo } from 'react';
import type { FC } from 'react';

import resets from '../../_resets.module.css';
import classes from './LinearDeterminateProgressIndic.module.css';

interface Props {
  className?: string;
  classes?: {
    track?: string;
    track2?: string;
    trackAndStop?: string;
    root?: string;
  };
}
/* @figmaId 158:992 */
export const LinearDeterminateProgressIndic: FC<Props> = memo(function LinearDeterminateProgressIndic(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div className={`${props.classes?.trackAndStop || ''} ${classes.trackAndStop}`}>
        <div className={`${props.classes?.track2 || ''} ${classes.track2}`}>
          <div className={`${props.classes?.track || ''} ${classes.track}`}></div>
        </div>
        <div className={classes.stop}>
          <div className={classes.stopShape}></div>
        </div>
      </div>
    </div>
  );
});
