@rem
@rem Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description:
@rem

call sbs -c armv5.test
call sbs freeze
call sbs -c armv5.test

call makesis UsbBasicPersonalityTest_ats.pkg
call signsis UsbBasicPersonalityTest_ats.sis UsbBasicPersonalityTest_ats.sisx x:\rd.cer x:\rd-key.pem
