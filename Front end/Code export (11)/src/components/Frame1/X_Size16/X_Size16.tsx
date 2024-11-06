import { memo } from 'react';
import type { FC } from 'react';

import resets from '../../_resets.module.css';
import { Size16Icon } from './Size16Icon.js';
import classes from './X_Size16.module.css';

interface Props {
  className?: string;
}
/* @figmaId 163:492 */
export const X_Size16: FC<Props> = memo(function X_Size16(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <div className={classes.icon}>
        <Size16Icon className={classes.icon2} />
      </div>
    </div>
  );
});
