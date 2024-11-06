import { memo } from 'react';
import type { FC } from 'react';

import resets from '../../_resets.module.css';
import { Settings } from '../Settings/Settings.js';
import { Icon } from './Icon.js';
import classes from './IconButton_StyleFilledStateEna.module.css';

interface Props {
  className?: string;
  classes?: {
    root?: string;
  };
}
/* @figmaId 158:706 */
export const IconButton_StyleFilledStateEna: FC<Props> = memo(function IconButton_StyleFilledStateEna(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div className={classes.container}>
        <div className={classes.stateLayer}>
          <Settings
            swap={{
              icon: <Icon className={classes.icon} />,
            }}
          />
        </div>
      </div>
    </div>
  );
});
